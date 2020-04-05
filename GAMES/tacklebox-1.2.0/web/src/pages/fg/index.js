import React from "react";
import classnames from "classnames";
import Helmet from "react-helmet";
import Layout from "../../components/tackleboxLayout";
import Fish from "../../components/fish";

import "./common.css";
import "./index.css";
import fgLogoImg from "../../images/fgLogo.png";

const fishData = require("../../../../fish.json");

class FishGuide extends React.Component {
    constructor(props) {
        super(props);

        this.state = {
            agreedToNotTell: false,
            okWithSpoilers: false
        };
    }

    render() {
        const { agreedToNotTell, okWithSpoilers } = this.state;

        const agreeToNotTellClasses = classnames("agreement", {
            agreed: agreedToNotTell
        });

        const okWithSpoilersClasses = classnames("agreement", {
            agreed: okWithSpoilers
        });

        let pageBody = null;

        if (agreedToNotTell && okWithSpoilers) {
            pageBody = (
                <React.Fragment>
                    {fishData.fish
                        .filter(f => f.name !== "OLD BOOT")
                        .map(fish => (
                            <Fish className="fish" data={fish} />
                        ))}
                </React.Fragment>
            );
        } else {
            pageBody = (
                <div>
                    <h1>Please check these to see the guide</h1>
                    <div className={agreeToNotTellClasses}>
                        <input
                            onClick={() =>
                                this.setState({ agreedToNotTell: true })
                            }
                            type="checkbox"
                        />
                        I won't tell other players about this page
                    </div>
                    <div className={okWithSpoilersClasses}>
                        <input
                            onClick={() =>
                                this.setState({ okWithSpoilers: true })
                            }
                            type="checkbox"
                        />
                        I am OK with spoilers
                    </div>
                </div>
            );
        }

        return (
            <>
                <Helmet>
                    <meta name="viewport" content="initial-scale=0.3" />
                </Helmet>
                <Layout className="fg" noHelmet={true}>
                    <img
                        className="titleImage"
                        src={fgLogoImg}
                        alt="Fish Guide Logo"
                    />
                    <div className="guideContainer">{pageBody}</div>
                    <p>&copy;2018 Matt Greer</p>
                </Layout>
            </>
        );
    }
}

export default FishGuide;
