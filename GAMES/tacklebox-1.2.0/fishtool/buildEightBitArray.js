const fs = require("fs");

function getFishWidth(name) {
    const files = fs.readdirSync("./nonMaskPngs");
    const file = files.find(f => f.startsWith(name));

    // GOLDFISH_8x8 -> ['GOLDFISH', '8x8'] -> 8
    return parseInt(file.split("_")[1], 10);
}

module.exports = function buildEightBitArray(fishes) {
    const entries = fishes.map(fish => {
        return `
    // ${fish.name}
    ${fish.activeHours[0]},  // minHour
    ${fish.activeHours[1]},  // maxHour
    ${fish.baitPreference.WORM},  // baitPreference WORM
    ${fish.baitPreference.GRUB},  // baitPreference GRUB
    ${fish.baitPreference.SHRIMP},  // baitPreference SHRIMP
    ${fish.baitPreference.MEAT},  // baitPreference MEAT
    ${fish.ratio},  // ratio
    ${fish.value},  // value
    ${fish.name.length}, // nameLength
    ${getFishWidth(fish.name)}, // bmpWidth
    ${fish.pull}, // pull
    ${(fish.deepWater && 1) || 0}, // deepWater
    ${fish.proWindow}, // proWindow`;
    });

    return `const uint8_t PROGMEM fish_templates_u8t[] = {
${entries.join("\n")}
};\n`;
};
