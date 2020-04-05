import React from "react";
import Layout from "../../components/tackleboxLayout";
import Hint from "../../components/hint";

import "./common.css";
import "./help.css";
import titleImg from "../../images/tackleboxTitle.png";
import dpadImg from "../../images/dpad.png";
import aButtonImg from "../../images/aButton.png";
import bButtonImg from "../../images/bButton.png";
import proFishingImg from "../../images/proFishing.png";

const HelpPage = () => (
    <Layout className="help">
        <img className="titleImage" src={titleImg} alt="Ardynia Title Screen" />
        <p>
            Armed with ten bucks, a fishing pole and a dream, your quest is to
            find and catch the big one.
        </p>
        <h2>Controls</h2>
        <div className="controlsContainer">
            <div>
                <img src={dpadImg} alt="directional pad" />
            </div>
            <div>move around</div>
            <div>
                hold <img src={aButtonImg} alt="the a button" />
            </div>
            <div>Pick a place to fish</div>
            <div>
                press <img src={bButtonImg} alt="the b button" />
            </div>
            <div>Open the game menu</div>
            <div>
                <img src={dpadImg} alt="directional pad" />
            </div>
            <div>
                Left/right toggle between haul and length records in the
                collection menu
            </div>
        </div>

        <div className="howToFish">
            <h2>How to fish: casual mode</h2>
            <p>Casual fishing is how the game starts out.</p>
            <p>
                Once a fish bites, mash on A until you have filled the progress
                bar
            </p>

            <h2>How to fish: pro mode</h2>
            <div className="alert">
                Pro mode is not available at first, you might need to talk to
                someone to get it enabled (hint hint)
            </div>
            <p>Pro fishing is a bit trickier.</p>
            <p>
                There are two progress bars, a big one and a little one. You
                need to keep the big progress bar's value inside the little one
                until the little one fills up.
            </p>
            <img
                className="proFishingImg"
                src={proFishingImg}
                alt="pro fishing"
            />
            <p>
                Here the player is winning, the big progress bar is right where
                it needs to be
            </p>
            <p>
                Pro fishing requires more focus and better simulates fishing.
                Pro fishing is challenging with the advanced fishes in the
                ocean.
            </p>
        </div>
        <h2>Hints</h2>
        <Hint>Having trouble finding worms? try hunting for them at night</Hint>
        <Hint>Ocean fish aren't tempted by worms, you'll need better bait</Hint>
        <Hint>Got another old boot? Bummer. But, they do have a use!</Hint>
        <Hint>
            Still need a few fish and just can't find them? Buy Bear's advice in
            the store
        </Hint>
    </Layout>
);

export default HelpPage;
