#include "stdafx.h"
#include "TileMap.h"

bool TileMap::load(const std::string& tileset, std::vector<std::tuple<int, int, int>>& tiles, unsigned int width, unsigned int height)
{
    // load the tileset texture
    if (!m_tileset.loadFromFile(tileset))
        return false;

    // Determine the width and height of the tilemap
    width = 0;
    height = tiles.size();

    if (!tiles.empty()) {
        width = tiles.size() / height;
    }

    // resize the vertex array to fit the level size
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(width * height * 4);

    // populate the vertex array, with one quad per tile
    for (unsigned int i = 0; i < width; ++i)
    {
        for (unsigned int j = 0; j < height; ++j)
        {
            // get the current tile number
            int tileNumber = std::get<0>(tiles[i + j * width]);
            int tileHeight = std::get<1>(tiles[i + j * width]);
            int tileWidth = std::get<2>(tiles[i + j * width]);

            // find its position in the tileset texture
            int tu = tileNumber % (m_tileset.getSize().x / tileWidth);
            int tv = tileNumber / (m_tileset.getSize().x / tileWidth);

            // get a pointer to the current tile's quad
            sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

            // define its 4 corners
            quad[0].position = sf::Vector2f(i * tileWidth, j * tileHeight);
            quad[1].position = sf::Vector2f((i + 1) * tileWidth, j * tileHeight);
            quad[2].position = sf::Vector2f((i + 1) * tileWidth, (j + 1) * tileHeight);
            quad[3].position = sf::Vector2f(i * tileWidth, (j + 1) * tileHeight);

            // define its 4 texture coordinates
            quad[0].texCoords = sf::Vector2f(tu * tileWidth, tv * tileHeight);
            quad[1].texCoords = sf::Vector2f((tu + 1) * tileWidth, tv * tileHeight);
            quad[2].texCoords = sf::Vector2f((tu + 1) * tileWidth, (tv + 1) * tileHeight);
            quad[3].texCoords = sf::Vector2f(tu * tileWidth, (tv + 1) * tileHeight);
        }
    }

    return true;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = &m_tileset;

    // draw the vertex array
    target.draw(m_vertices, states);
}
