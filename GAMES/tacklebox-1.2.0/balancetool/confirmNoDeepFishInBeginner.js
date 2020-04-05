module.exports = function confirmNoDeepFishInBeginner(fishData) {
    fishData.fish.forEach(f => {
        if (f.deepWater && f.habitats.includes("BEGINNER")) {
            throw new Error(`${fish.name} is in BEGINNER and deep`);
        }
    });
};
