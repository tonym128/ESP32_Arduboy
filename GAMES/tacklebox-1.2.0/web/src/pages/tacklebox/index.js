import React from "react";
import Layout from "../../components/tackleboxLayout";

import "./common.css";
import "./index.css";
import titleImg from "../../images/tackleboxTitle.png";
import collectionMenuMp4 from "../../images/collectionMenu.mp4";
import catchLobsterMp4 from "../../images/catchLobster.mp4";
import bearsTackleImg from "../../images/bearsTackle.png";
import buyInTheShopImg from "../../images/buyInTheShop.png";

const IndexPage = () => (
    <Layout className="home">
        <img
            className="titleImage"
            src={titleImg}
            alt="Tackle Box Title Screen"
        />
        <p>
            A fishing adventure in your pocket. Can you find all 20 fish? A
            casual Arduboy game, inspired by Animal Crossing.
        </p>

        <a
            href="https://github.com/city41/tacklebox/releases"
            className="downloadHere"
        >
            Download Here
        </a>

        <div className="mediaContainer">
            <video
                className="catchLobster"
                src={catchLobsterMp4}
                muted
                width="256"
                height="128"
                autoPlay
                loop
                playsinline
                controls
            />
            <div className="divider" />
            <video
                className="collectionMenu"
                src={collectionMenuMp4}
                muted
                width="256"
                height="128"
                autoPlay
                loop
                playsinline
                controls
            />
            <img
                className="bearsTackle"
                src={bearsTackleImg}
                alt="outside bear's tackle shop"
            />
            <img
                className="buyInTheShop"
                src={buyInTheShopImg}
                alt="buying things in the shop"
            />
        </div>

        <div className="features">
            <h3>Features</h3>
            <ul>
                <li>20 fish to catch</li>
                <li>
                    a day/night cycle, different things happen at different
                    times of the day
                </li>
                <li>Five bodies of water to fish in</li>
                <li>
                    Different fishes prefer different baits, are active at
                    different hours, etc
                </li>
                <li>A couple secrets to find</li>
                <li>
                    A full featured shop to buy equipment and sell your haul
                </li>
            </ul>
        </div>
        <p>&copy;2018 Matt Greer</p>
    </Layout>
);

export default IndexPage;
