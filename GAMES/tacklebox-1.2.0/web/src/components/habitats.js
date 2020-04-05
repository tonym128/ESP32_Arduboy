import React from "react";

import styles from "./habitats.module.css";

const nameMap = {
    CORNER: "the river",
    ADVANCED: "advanced pond",
    BEGINNER: "beginner pond",
    OCEAN: "the ocean",
    OCEAN_ISLAND: "island"
};

export default ({ habitats, deepWater }) => {
    return (
        <div className={styles.habitats}>
            <h5 className={styles.foundIn}>found in:</h5>
            {habitats.map(h => (
                <div>
                    <img src={`/fishImages/${h}.png`} alt={nameMap[h]} />
                    {nameMap[h]} {deepWater && "(deep water)"}
                </div>
            ))}
        </div>
    );
};
