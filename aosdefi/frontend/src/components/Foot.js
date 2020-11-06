import { Flex } from "antd-mobile";
import React from "react";

function Main() {
  return (
    <>
      <Flex justify="center" className="footer">
        <Flex>
          <a href="https://twitter.com/aoswap" className="app-link">
            Twitter
          </a>
          <a
            href="https://discord.gg/MS7nnXH"
            className="app-link"
            style={{ margin: "0px 28px" }}
          >
            Discord
          </a>
          <a
            href="https://docs.google.com/document/d/1EMVCvPSDu9XcBZQ9xt2Epi_QVaPVzNcBDrK8KlweYts/edit?usp=sharing"
            className="app-link"
          >
            WhitePaper
          </a>
        </Flex>
      </Flex>
    </>
  );
}

export default Main;
