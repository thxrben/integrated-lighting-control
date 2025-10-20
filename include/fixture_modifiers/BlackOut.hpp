#include "fixture_modifiers/FixtureModifier.hpp"

#ifndef BLACKOUT_MODIFIER_HPP
#define BLACKOUT_MODIFIER_HPP

class BlackOut : public FixtureModifier {

    public:
        BlackOut(std::shared_ptr<Fixture[]> fixtures);
        virtual void modify();

};


#endif