#pragma once

#include "Platform.h"

class TileMap
	: public sf::Drawable, public sf::Transformable
{
public:

    bool load(const std::string& tileset, std::vector<std::tuple<int, int, int>>& tiles, unsigned int width, unsigned int height);

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};

