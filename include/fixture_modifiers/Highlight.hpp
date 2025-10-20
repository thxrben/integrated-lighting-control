#include "fixture_modifiers/FixtureModifier.hpp"

#ifndef HIGHLIGHT_MODIFIER_HPP
#define HIGHLIGHT_MODIFIER_HPP

class Highlight : public FixtureModifier {

    public:
        Highlight(std::shared_ptr<Fixture[]> fixtures);
        virtual void modify();

};


#endif