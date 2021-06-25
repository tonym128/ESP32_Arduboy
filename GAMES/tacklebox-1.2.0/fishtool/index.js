const fs = require("fs");
const path = require("path");
const program = require("commander");
const packageJson = require("./package.json");
const buildEightBitArray = require("./buildEightBitArray");
const buildSixteenBitArray = require("./buildSixteenBitArray");
const buildFishTypes = require("./buildFishTypes");

program
    .version(packageJson.version)
    .option("-s, --src <fish file>", "The json file containing the fish data")
    .option(
        "-o, --dest <dest dir>",
        "The directory to write the header files to"
    )
    .parse(process.argv);

if (!program.src) {
    console.error(
        "usage: node ./fishtool/ -s <fish json file> -o <header output file path>"
    );
    process.exit(1);
}

const fishJson = require(path.join(process.cwd(), program.src));

const eightBitData = buildEightBitArray(fishJson.fish);
const sixteenBitData = buildSixteenBitArray(fishJson.fish);

const constants = `
#include "nonMaskBitmaps.h"

const uint8_t NUM_U8T_PROPS = 13;
const uint8_t NUM_16T_PROPS = 6;
`;

const fileData =
    "#pragma once\n\n" +
    constants +
    "\n\n" +
    eightBitData +
    "\n\n" +
    sixteenBitData;

const destPath = program.dest
    ? path.join(process.cwd(), program.dest, "fishTemplates.h")
    : null;

if (destPath) {
    fs.writeFileSync(destPath, fileData);
    console.log("wrote: ", destPath);
} else {
    console.log(fileData);
}

const fishTypeData = buildFishTypes(fishJson.fish);

const typeDestPath = program.dest
    ? path.join(process.cwd(), program.dest, "fishType.h")
    : null;

if (typeDestPath) {
    fs.writeFileSync(typeDestPath, fishTypeData);
    console.log("wrote: ", typeDestPath);
} else {
    console.log(fishTypeData);
}
