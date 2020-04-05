module.exports = function buildMapArray(name, layer, tileSize) {
    const { data, width, height } = layer;

    const zeroBasedData = data.map(d => d - 1);

    const arrayLength = zeroBasedData.length;

    return `
const int16_t TILE_SIZE = 16;
const int16_t MAP_WIDTH_TILES = ${width};
const int16_t MAP_HEIGHT_TILES = ${height};
const int16_t MAP_WIDTH_PX = MAP_WIDTH_TILES * TILE_SIZE;
const int16_t MAP_HEIGHT_PX = MAP_HEIGHT_TILES * TILE_SIZE;

const uint8_t PROGMEM ${name}_map[MAP_WIDTH_TILES * MAP_HEIGHT_TILES] = {
    ${zeroBasedData.map(n => "0x" + n.toString(16)).join(", ")}
};`;
};
