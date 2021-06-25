module.exports = function buildWorms(objectLayer) {
    const worms = objectLayer.objects
        .filter(obj => obj.type === "WORM_SPAWN")
        .map(obj => {
            return `Worm(${Math.round(obj.x)}, ${Math.round(obj.y)})`;
        });

    const wormString = worms.join(",\n\t");

    return `const uint8_t MAX_WORMS = ${
        worms.length
    };\n\nWorm worms[MAX_WORMS] = {\n\t${wormString}\n};\n\n`;
};
