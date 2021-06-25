const confirmNoDeepFishInBeginner = require("./confirmNoDeepFishInBeginner");
const buildTable = require("./buildTable");
const fishData = require("../fish.json");

confirmNoDeepFishInBeginner(fishData);

const habitats = Object.freeze([
    "CORNER",
    "CORNER_deep",
    "ADVANCED",
    "ADVANCED_deep",
    "BEGINNER",
    "OCEAN",
    "OCEAN_ISLAND",
    "OCEAN_ISLAND_deep"
]);

const bait = Object.freeze(["WORM", "GRUB", "SHRIMP", "MEAT"]);

let hours = new Array(24);
hours.fill(0, 0, 24);

function countFishFor(habitat, hour, bait) {
    const habitatName = habitat.replace("_deep", "");
    const isDeep = habitat.includes("_deep");

    const result = fishData.fish.reduce(
        (buildingObj, f) => {
            if (
                !!f.deepWater === isDeep &&
                f.baitPreference[bait] === 1 &&
                f.habitats.includes(habitatName) &&
                f.activeHours[0] <= hour &&
                f.activeHours[1] >= hour
            ) {
                buildingObj.count += 1;
                buildingObj.ratio += f.ratio;
            }

            return buildingObj;
        },
        { count: 0, ratio: 0 }
    );

    return result;
}

hours = hours.map((_, index) => {
    let hour = {};
    hour.hour = index;
    habitats.forEach(hab => {
        hour[hab] = {};

        bait.forEach(b => {
            hour[hab][b] = countFishFor(hab, hour.hour, b);
        });
    });

    return hour;
});

const tableHtml = buildTable(hours, habitats);

console.log(tableHtml);
