import React from "react";
import classnames from "classnames";
import Habitats from "./habitats";

import styles from "fish.module.css";

function getLikes(baitPreferences) {
    const likes = Object.keys(baitPreferences).reduce((building, key) => {
        if (baitPreferences[key]) {
            return building.concat(key);
        } else {
            return building;
        }
    }, []);

    return likes.join(", ").toLowerCase();
}

function getActiveFrom(hours) {
    if (hours[0] === 0 && hours[1] === 23) {
        return "all day";
    } else {
        const [startHour, endHour] = hours;

        const startHourIsPm = startHour >= 12;
        const endHourIsPm = endHour >= 12;

        const startTime = startHour > 12 ? startHour - 12 : startHour;
        const endTime = endHour > 12 ? endHour - 12 : endHour;

        const start = `${startTime === 0 ? "12" : startTime}${(startHourIsPm &&
            "pm") ||
            "am"}`;
        const end = `${endTime}${(endHourIsPm && "pm") || "am"}`;

        return `from ${start} to ${end}`;
    }
}

export default ({ className, data }) => {
    const classes = classnames(className, styles.fish);

    return (
        <div className={classes}>
            <div className={styles.nameAndImage}>
                <h4>{data.name}</h4>
                <div className={styles.imgContainer}>
                    <img
                        className={styles[data.name]}
                        src={`/fishImages/${data.name}.png`}
                        alt={data.name}
                    />
                </div>
            </div>
            <div className={styles.data}>
                <Habitats habitats={data.habitats} deepWater={data.deepWater} />
                <div className={styles.stats}>
                    <div>active {getActiveFrom(data.activeHours)}</div>
                    <div>likes: {getLikes(data.baitPreference)}</div>
                    <div>
                        size range: {data.lengths[0]}
                        cm to {data.lengths[1]}
                        cm{" "}
                    </div>
                    <div>value: ${data.value} </div>
                </div>
            </div>
        </div>
    );
};
