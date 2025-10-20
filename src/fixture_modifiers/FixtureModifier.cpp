#include "fixture_modifiers/FixtureModifier.hpp"

FixtureModifier::FixtureModifier() {
    FixtureModifier(nullptr);
}

FixtureModifier::FixtureModifier(std::shared_ptr<Fixture[]> fixtures) {
    this->fixtures = fixtures;

    if(fixtures == nullptr)
        printf("[MODIFIER] (WARN) - No fixtures to modify\n");
};