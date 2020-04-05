import React from "react";
import classnames from "classnames";
import PropTypes from "prop-types";
import Helmet from "react-helmet";
import { StaticQuery, graphql } from "gatsby";

import Header from "./header";
import "./layout.css";
const titleImageUrl = "https://city41.games/tackleboxTitle.png";

const PAGES = [
    { url: "/tacklebox", label: "home" },
    { url: "/tacklebox/help", label: "help" },
    { url: "/tacklebox/contact", label: "contact" }
];

const Layout = ({ className, children }) => {
    const classes = classnames("layout", className);
    return (
        <StaticQuery
            query={graphql`
                query TackleBoxSiteTitleQuery {
                    site {
                        siteMetadata {
                            tacklebox {
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
                        title={data.site.siteMetadata.tacklebox.title}
                        meta={[
                            {
                                name: "description",
                                content: data.site.siteMetadata.tacklebox.title
                            },
                            {
                                name: "keywords",
                                content:
                                    data.site.siteMetadata.tacklebox.keywords
                            },
                            {
                                name: "twitter:card",
                                content: "summary"
                            },
                            {
                                name: "twitter:site",
                                content:
                                    data.site.siteMetadata.tacklebox
                                        .twitterHandle
                            },
                            {
                                name: "twitter:title",
                                content: data.site.siteMetadata.tacklebox.title
                            },
                            {
                                name: "twitter:description",
                                content:
                                    data.site.siteMetadata.tacklebox.description
                            },
                            {
                                name: "twitter:image",
                                content: titleImageUrl
                            },
                            {
                                name: "og:title",
                                content: data.site.siteMetadata.tacklebox.title
                            },
                            {
                                name: "og:type",
                                content: "website"
                            },
                            {
                                name: "og:url",
                                content: "https://city41.games/tacklebox"
                            },
                            {
                                name: "og:description",
                                content:
                                    data.site.siteMetadata.tacklebox.description
                            },
                            {
                                name: "og:image",
                                content: titleImageUrl
                            },
                            {
                                name: "og:title",
                                content: data.site.siteMetadata.tacklebox.title
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
