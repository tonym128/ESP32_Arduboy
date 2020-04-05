const fs = require("fs");
const path = require("path");
const program = require("commander");
const packageJson = require("./package.json");
const buildMapArray = require("./buildMapArray");
const buildWorms = require("./buildWorms");

function findLayer(layers, prop) {
    return layers.find(l => !!l[prop]);
}

function findEntityLayer(layers) {
    return layers.find(l => !!l.objects && l.name == "entities");
}

program
    .version(packageJson.version)
    .option(
        "-s, --src <map file directory>",
        "A directory containing json files which are exported from Tiled"
    )
    .option(
        "-o, --dest <dest directory>",
        "The directory to write the header files to"
    )
    .parse(process.argv);

if (!program.src) {
    console.error(
        "usage: node ./maptool/ -s <map file directory> -o <header output directory>"
    );
    process.exit(1);
}

function createWormsFile(tiledJson, outDir) {
    const entityLayer = findEntityLayer(tiledJson.layers);
    const wormData = buildWorms(entityLayer);
    const fileData = "#pragma once\n\n" + wormData;

    if (outDir) {
        const destPath = path.join(outDir, "worms.h");
        fs.writeFileSync(destPath, fileData);
        console.log("wrote: ", destPath);
    } else {
        console.log(wormData);
    }
}

function createHeaderFile(tiledJson, baseName, outDir) {
    const mapArrayString = buildMapArray(
        baseName,
        findLayer(tiledJson.layers, "data"),
        tiledJson.tilewidth
    );

    const entityLayer = findEntityLayer(tiledJson.layers);

    if (!entityLayer) {
        throw new Error(
            'must have an object layer for entities named "entities"'
        );
    }

    const pragma = "#pragma once";

    const finalFile = pragma + "\n\n" + mapArrayString;

    if (outDir) {
        const destPath = path.join(outDir, baseName + ".h");
        fs.writeFileSync(destPath, finalFile);
        console.log("wrote: ", destPath);
    } else {
        console.log(finalFile);
    }
}

function isTiledJson(file) {
    if (path.extname(file) !== ".json") {
        return false;
    }

    try {
        const tiledJson = require(file);
        return tiledJson.type === "map";
    } catch (e) {
        return false;
    }
}

const tiledJsonDir = path.join(process.cwd(), program.src);
const outDir = program.dest ? path.join(process.cwd(), program.dest) : null;

const tiledJsonFiles = fs.readdirSync(tiledJsonDir);

tiledJsonFiles.forEach(file => {
    const fullPath = path.join(process.cwd(), program.src, file);

    if (isTiledJson(fullPath)) {
        console.log("about to process", fullPath);
        const tiledJson = require(fullPath);
        const baseName = path.basename(file, ".json");
        createHeaderFile(tiledJson, baseName, outDir);
        createWormsFile(tiledJson, outDir);
    }
});

if (outDir) {
    console.log("all header files written to", outDir);
}
