#include <memory>
#include <stdio.h>

#include "fixture/Fixture.hpp"

#ifndef FIXTURE_MODIFIER_HPP
#define FIXTURE_MODIFIER_HPP

class FixtureModifier {
    protected:
        std::shared_ptr<Fixture[]> fixtures;
    public:        
        FixtureModifier();               
        FixtureModifier(std::shared_ptr<Fixture[]> fixtures);
        virtual ~FixtureModifier() = default;
        virtual void modify();
};

#endif
