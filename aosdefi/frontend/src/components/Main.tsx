import React, { useState, useEffect } from "react";
import { Flex, Toast, ActivityIndicator } from "antd-mobile";
import "../styles/Home.css";
import logo_small from "../assets/logo_small@2x.png";
import Home from "./Home";
import Crops from "./Crops";
import Team from "./Team";
import Foot from "./Foot";
import { createPool, login } from "../api/aosService";
import OpenLandModal from "./OpenLandModal";
import MiddleBackground from "../assets/home_middle_bg@2x.png";
import slogan from "../assets/slogan@2x.png";
import logoBig from "../assets/logo_big@2x.png";
function Main() {
  const items = ["Home", "My Crops", "My team"];

  const [page, setPage] = useState(0);
  const [openVisible, setOpenVisible] = useState(false);
  const [animating, setAnimating] = useState(false);
  const [account, setAccount] = useState();
  const [reload, setReload] = useState(false);
  useEffect(() => {
    login().then((account) => {
      if (account === false || account === undefined) {
        Toast.info("not scatter");
        // sessionStorage.removeItem("account");
        return;
      }
      setAccount(account);
      // sessionStorage.setItem("account", account);
    });
  }, []);
  return (
    <div className="mainRoot">
      <div className="mainController">
        {page === 0 && (
          <Flex className="topContainer" direction="column" justify="center">
            <img src={logoBig} width={130} alt="logo" />
            <img
              style={{ marginTop: 21 }}
              src={slogan}
              className="slogan"
              alt="slogan"
            />
            <div style={{ color: "#642C20" }}>
              Farm all the AOS DeFi tokens all in one!
            </div>
          </Flex>
        )}

        <img
          src={MiddleBackground}
          width="100%"
          style={{ marginTop: -10 }}
          alt="middle"
        />
        {page === 0 && <Home account={account} reload={reload} />}
        {page === 1 && <Crops account={account} />}
        {page === 2 && <Team account={account} />}
        <Foot />
      </div>
      {openVisible && (
        <OpenLandModal
          visible={openVisible}
          closeAction={() => setOpenVisible(false)}
          doneAction={(result: any) => {
            const {
              depositContract,
              amount,
              deposit,
              factor,
              mining,
              miningContract,
              day,
            } = result;
            setAnimating(true);
            const memo = `CREATE:${depositContract}|4,${deposit}|${factor}|${day}`;
            createPool(account, miningContract, mining, amount, memo)
              .then((trx) => {
                //进入结果页面
                setAnimating(false);
                Toast.success("success");
                setOpenVisible(false);
                setReload(!reload);
              })
              .catch((err) => {
                setAnimating(false);
                const errData = JSON.parse(err);
                if (errData.error) {
                  const message = errData.error.details[0].message;
                  Toast.info(message, 3);
                }
              });
          }}
        />
      )}

      <ActivityIndicator toast text="loading" animating={animating} />
      <Flex className="header" justify="between">
        <Flex style={{ marginLeft: 15 }}>
          <img src={logo_small} className="logo" alt="logo" />
          <Flex style={{ marginLeft: 15 }}>
            {items.map((item, index) => {
              return (
                <div
                  key={index}
                  className={index === page ? "item_selected" : "item_normal"}
                  onClick={() => setPage(index)}
                >
                  {item}
                </div>
              );
            })}
          </Flex>
        </Flex>
        <div
          className="openButton"
          style={{ marginRight: 15 }}
          onClick={() => setOpenVisible(true)}
        >
          Open up land
        </div>
      </Flex>
    </div>
  );
}

export default Main;
