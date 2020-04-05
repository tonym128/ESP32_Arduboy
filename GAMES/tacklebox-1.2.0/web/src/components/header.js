import React from "react";
import { Link } from "gatsby";
import city41LogoSmallImg from "../images/city41LogoSmall.png";

import "./header.css";

const Header = ({ pages }) => (
    <nav className="header">
        <ul>
            <li className="city41Home">
                <Link to="/">
                    <img
                        src={city41LogoSmallImg}
                        alt="small city41 games logo"
                    />
                </Link>
            </li>
            {pages.map(p => (
                <li>
                    <Link to={p.url}>{p.label}</Link>
                </li>
            ))}
        </ul>
    </nav>
);

export default Header;
