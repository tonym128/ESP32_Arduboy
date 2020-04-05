const minXes = {
    CORNER: 0,
    ADVANCED: 5 * 16,
    BEGINNER: 22 * 16,
    OCEAN: 35 * 16,
    OCEAN_ISLAND: 38 * 16
};

const maxXes = {
    CORNER: 4 * 16,
    ADVANCED: 18 * 16,
    BEGINNER: 27 * 16,
    OCEAN: 38 * 16,
    OCEAN_ISLAND: 50 * 16
};

module.exports = {
    getMinX(habitats) {
        const mins = Object.keys(minXes).reduce((building, k) => {
            if (habitats.indexOf(k) > -1) {
                return building.concat(minXes[k]);
            } else {
                return building;
            }
        }, []);

        return Math.min(...mins);
    },

    getMaxX(habitats) {
        const maxes = Object.keys(maxXes).reduce((building, k) => {
            if (habitats.indexOf(k) > -1) {
                return building.concat(maxXes[k]);
            } else {
                return building;
            }
        }, []);

        return Math.max(...maxes);
    }
};
