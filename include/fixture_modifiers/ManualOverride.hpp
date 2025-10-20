#include "fixture_modifiers/FixtureModifier.hpp"

#ifndef MANUAL_OVERRIDE_MODIFIER_HPP
#define MANUAL_OVERRIDE_MODIFIER_HPP

class ManualOverride : public FixtureModifier {

    public:
        ManualOverride(std::shared_ptr<Fixture[]> fixtures);
        virtual void modify();

};


#endif