import React from "react";
import { Link } from "gatsby";

import tackleboxImg from "../images/tackleboxTitle.png";
import ardyniaImg from "../images/ardyniaTitle.png";

import "../components/layout.css";
import style from "index.module.css";

const RootIndexPage = () => (
    <div className={style.root}>
        <div className={style.logo} />
        <div className={style.gameTitle}>
            <Link to="/tacklebox">
                <img src={tackleboxImg} alt="Tackle Box title" />
            </Link>
        </div>
        <div className={style.gameTitle}>
            <Link to="/ardynia">
                <img src={ardyniaImg} alt="Ardynia title" />
            </Link>
        </div>
    </div>
);

export default RootIndexPage;
