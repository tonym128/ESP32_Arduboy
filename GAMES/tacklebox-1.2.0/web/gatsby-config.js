module.exports = {
    siteMetadata: {
        title: "City41 Games",
        keywords:
            "Arduboy, games, animal crossing style, zelda style, fishing, adventure",
        description: "I make games for the Arduboy",
        twitterHandle: "@mattegreer",
        ardynia: {
            title: "Ardynia",
            keywords:
                "Arduboy, games, animal crosing style, fishing, adventure",
            description: "A Zelda style adventure game for the Arduboy"
        },
        tacklebox: {
            title: "Tackle Box",
            keywords:
                "Arduboy, games, animal crosing style, fishing, adventure",
            description: "A fishing adventure game for the Arduboy"
        }
    },
    plugins: [
        "gatsby-plugin-react-helmet",
        {
            resolve: `gatsby-plugin-manifest`,
            options: {
                name: "City41 Games",
                short_name: "City41",
                start_url: "/",
                background_color: "#663399",
                theme_color: "#663399",
                display: "minimal-ui",
                icon: "src/images/bearForFavicon.png" // This path is relative to the root of the site.
            }
        },
        "gatsby-plugin-offline",
        {
            resolve: "gatsby-plugin-favicon",
            options: {
                logo: "./src/images/bearForFavicon.png",

                // WebApp Manifest Configuration
                appName: null, // Inferred with your package.json
                appDescription: null,
                developerName: null,
                developerURL: null,
                dir: "auto",
                lang: "en-US",
                background: "#fff",
                theme_color: "#fff",
                display: "standalone",
                orientation: "any",
                start_url: "/?homescreen=1",
                version: "1.0",

                icons: {
                    android: true,
                    appleIcon: true,
                    appleStartup: true,
                    coast: false,
                    favicons: true,
                    firefox: true,
                    opengraph: false,
                    twitter: false,
                    yandex: false,
                    windows: false
                }
            }
        }
    ]
};
