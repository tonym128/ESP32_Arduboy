import React from "react";
import classnames from "classnames";
import PropTypes from "prop-types";
import Helmet from "react-helmet";
import { StaticQuery, graphql } from "gatsby";

import Header from "./header";
import "./layout.css";
const titleImageUrl = "https://city41.games/ardyniaTitle.png";

const PAGES = [
    { url: "/ardynia", label: "home" },
    { url: "/ardynia/help", label: "help" },
    { url: "/ardynia/contact", label: "contact" }
];

const Layout = ({ className, children }) => {
    const classes = classnames("layout", className);
    return (
        <StaticQuery
            query={graphql`
                query ArdyniaSiteTitleQuery {
                    site {
                        siteMetadata {
                            ardynia {
                                title
                                keywords
                                description
                            }
                            twitterHandle
                        }
                    }
                }
            `}
            render={data => (
                <>
                    <Helmet
                        title={data.site.siteMetadata.ardynia.title}
                        meta={[
                            {
                                name: "description",
                                content: data.site.siteMetadata.ardynia.title
                            },
                            {
                                name: "keywords",
                                content: data.site.siteMetadata.ardynia.keywords
                            },
                            {
                                name: "twitter:card",
                                content: "summary"
                            },
                            {
                                name: "twitter:site",
                                content:
                                    data.site.siteMetadata.ardynia.twitterHandle
                            },
                            {
                                name: "twitter:title",
                                content: data.site.siteMetadata.ardynia.title
                            },
                            {
                                name: "twitter:description",
                                content:
                                    data.site.siteMetadata.ardynia.description
                            },
                            {
                                name: "twitter:image",
                                content: titleImageUrl
                            },
                            {
                                name: "og:title",
                                content: data.site.siteMetadata.ardynia.title
                            },
                            {
                                name: "og:type",
                                content: "website"
                            },
                            {
                                name: "og:url",
                                content: "https://city41.games/ardynia"
                            },
                            {
                                name: "og:description",
                                content:
                                    data.site.siteMetadata.ardynia.description
                            },
                            {
                                name: "og:image",
                                content: titleImageUrl
                            },
                            {
                                name: "og:title",
                                content: data.site.siteMetadata.ardynia.title
                            }
                        ]}
                    >
                        <html lang="en" />
                    </Helmet>
                    <Header pages={PAGES} />
                    <div className="layoutContainer">
                        <div className={classes}>{children}</div>
                    </div>
                </>
            )}
        />
    );
};

Layout.propTypes = {
    children: PropTypes.node.isRequired
};

export default Layout;
