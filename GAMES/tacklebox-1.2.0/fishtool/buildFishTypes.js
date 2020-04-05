module.exports = function buildFishTypes(fishes) {
    const entries = fishes.map((fish, index) => {
        return `    ${fish.name.replace(/ /g, "_")} = ${index}`;
    });

    entries.unshift("    UNSET = -1");
    entries.push(`    COUNT = ${fishes.length}`);

    return `#pragma once

enum class FishType: int8_t {
${entries.join(",\n")}
};`;
};
