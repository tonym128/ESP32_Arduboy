const encoding = require("../stringtool/encoding");
const { getMinX, getMaxX } = require("./getMinMaxXFromHabitats");

function buildActualSixteenBitArray(fishes) {
    const entries = fishes.map(fish => {
        const name = fish.name.replace(/ /g, "_");
        return `
    // ${fish.name}
    ${getMinX(fish.habitats)}, // min X
    ${getMaxX(fish.habitats)}, // max X
    reinterpret_cast<int16_t>(${name}_string),
    reinterpret_cast<int16_t>(${name}_tiles),
    ${fish.lengths[0]}, // minLength;
    ${fish.lengths[1]}, // maxLength`;
    });

    return `const int16_t PROGMEM fish_templates_16t[] = {
${entries.join("\n")}
};\n`;
}

function buildNames(fishes) {
    const nameArrays = fishes.map(fish => {
        const name = fish.name.replace(/ /g, "_");

        const values = fish.name.split("").map(c => encoding[c]);
        return `const uint8_t ${name}_string[${fish.name.length +
            1}] PROGMEM = {
    ${values.join(", ")}, 0xFF
};`;
    });

    return nameArrays.join("\n\n");
}

module.exports = function buildSixteenBitArray(fishes) {
    const names = buildNames(fishes);
    const sixteenBitArray = buildActualSixteenBitArray(fishes);

    return names + "\n\n" + sixteenBitArray;
};
