#include "StandardTest.hpp"

namespace
{
sol::state initLua()
{
    sol::state lua;
    lua.open_libraries(sol::lib::base);
    aot::lua::openCoreLibrary(lua);
    
    lua.safe_script("function p(a) return a end");
    lua.safe_script("function g() return val end");
    return lua;
}

std::string midiMessageToString(const MidiMessage& message)
{
    return std::string((char*)message.getRawData(), message.getRawDataSize());
}

bool isNoteOnNative(const MidiMessage& message)
{
    return message.isNoteOn();
}

bool isNoteOn(const std::string& midiString)
{
    MidiMessage message(midiString.data(), (int)midiString.size());
    return message.isNoteOn();
}

bool isNoteOn2(const std::string& midiString)
{
    auto data = midiString.data();
    return (data[0] & 0xf0) == 0x90;
}

}

TEST_CASE("Midi")
{
    SECTION("Read Message")
    {
        auto lua = initLua();
//        using midi_string = std::basic_string<uint8>;
        using midi_string = std::string;
        auto noteOn = MidiMessage::noteOn(1, 64, 1.0f);
        midi_string ms ((char*)noteOn.getRawData(), noteOn.getRawDataSize());
        
        std::cout << sizeof(typename std::string::value_type) << std::endl;
        lua["noteOn"] = ms;
        
        midi_string readMs = lua["noteOn"];
        auto readNote = MidiMessage(readMs.data(), readMs.size());
        REQUIRE(readNote.getChannel() == 1);
        REQUIRE(readNote.getNoteNumber() == 64);
    }
    
    SECTION("Feed midi message")
    {
        auto lua = initLua();
        auto midiMessageType = lua.new_usertype<MidiMessage>("MidiMessage");
        midiMessageType["isNoteOn"] = [](const MidiMessage& message) { return message.isNoteOn(); };
        midiMessageType["noteNumber"] = &MidiMessage::getNoteNumber;
        midiMessageType["channel"] = &MidiMessage::getChannel;
        midiMessageType["velocity"] = &MidiMessage::getVelocity;
        
        lua["isNoteOnNative"] = &isNoteOnNative;
        lua["isNoteOn"] = &isNoteOn;
        lua["isNoteOn2"] = &isNoteOn2;
        lua.safe_script(R"(
            numNoteOn = 0
            function handleMessage(m)
                if isNoteOnNative(m) then
                    numNoteOn = numNoteOn + 1;
                end
            end
            function handleMessageString(m)
                if isNoteOn(m) then
                    numNoteOn = numNoteOn + 1;
                end
            end
            function handleMessageString2(m)
                if isNoteOn2(m) then
                    numNoteOn = numNoteOn + 1;
                end
            end
        )");
        
        sol::function handleMessage = lua["handleMessage"];
        sol::function handleMessageString = lua["handleMessageString"];
        sol::function handleMessageString2 = lua["handleMessageString2"];
        
        BENCHMARK_ADVANCED("handleMessage: user type")(Catch::Benchmark::Chronometer meter)
        {
            meter.measure([&handleMessage] {
                handleMessage(MidiMessage::noteOn(1, 64, 1.0f));
            });
        };

        BENCHMARK_ADVANCED("handleMessage: string")(Catch::Benchmark::Chronometer meter)
        {
            meter.measure([&handleMessageString] {
                handleMessageString(midiMessageToString(MidiMessage::noteOn(1, 64, 1.0f)));
            });
        };
        
        BENCHMARK_ADVANCED("handleMessage2: string")(Catch::Benchmark::Chronometer meter)
        {
            meter.measure([&handleMessageString2] {
                handleMessageString2(midiMessageToString(MidiMessage::noteOn(1, 64, 1.0f)));
            });
        };
        
        int numNoteOn = lua["numNoteOn"];
//        REQUIRE(numNoteOn == 1);

        std::cout << std::endl << " NumNoteOn: " << numNoteOn << std::endl;
        
    }
}
