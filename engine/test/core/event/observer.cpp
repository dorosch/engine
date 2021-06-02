#include <catch2/catch.hpp>

#include "core/event/observer.hpp"

using namespace Engine::Event;


static EventType eventType;


void event_callback(Event *event) {
    eventType = event->type;
}


TEST_CASE("EventObserver", "[core][event]") {
    SECTION("singleton implementation") {
        REQUIRE(EventObserver::GetInstance() == EventObserver::GetInstance());
    }

    SECTION("subscribe and get result from callback") {
        EventObserver::GetInstance()->Subscribe(event_callback);

        EventObserver::GetInstance()->Publish(new Event(EventType::Mouse));
        REQUIRE(eventType == EventType::Mouse);

        EventObserver::GetInstance()->Publish(new Event(EventType::Keyboard));
        REQUIRE(eventType == EventType::Keyboard);

        EventObserver::GetInstance()->Unsubscribe(event_callback);
    }

    SECTION("unsubscribe from subscribers") {
        eventType = EventType::Keyboard;

        EventObserver::GetInstance()->Subscribe(event_callback);
        EventObserver::GetInstance()->Unsubscribe(event_callback);

        EventObserver::GetInstance()->Publish(new Event(EventType::Mouse));
        REQUIRE(eventType == EventType::Keyboard);
    }
}
