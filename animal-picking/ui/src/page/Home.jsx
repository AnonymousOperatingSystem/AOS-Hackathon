import React, { useEffect, useState } from "react";
import styles from "../style/Home.module.scss";
import InputModal from "../component/InputModal";
import { Toast } from "antd-mobile";
import { Backdrop, CircularProgress, List } from "@material-ui/core";
import { animals } from "../assets/animals/animals";
import { isRealNum } from "../utils";
import { handleTransfer, login } from "../service/aosService";
import { tokens } from "../service/config";
import { useHistory } from "react-router-dom";
import { getGlobalInfo, getPeriodInfo } from "../service/rpcService";
import intl from "react-intl-universal";

export default function Home() {
  const [selectedItem, setSelectedItem] = useState(-1);
  const [modalOpen, setModalOpen] = useState(false);
  const [loading, setLoading] = useState(true);

  const [account, setAccount] = useState({
    name: "",
  });
  const [ranks, setRanks] = useState([]);
  const [period, setPeriod] = useState(0);
  const history = useHistory();
  useEffect(() => {
    login()
      .then((account) => {
        setLoading(false);
        if (account === false || account === undefined) {
          Toast.fail(intl.get("Tip_NoScatter"));
          return;
        }
        setAccount(account);
      })
      .catch((error) => setLoading(false));
    getRanks();
  }, []);

  function getRanks() {
    //获取排行榜
    getGlobalInfo().then((resp) => {
      const rows = resp.rows;
      if (rows.length > 0) {
        //当前期数
        const period = rows[0].period;
        setPeriod(period);
        getPeriodInfo(period)
          .then((info) => info.rows)
          .then((data) => {
            if (data.length > 0) {
              const animals = data[0].animals;

              animals.sort(function (a, b) {
                return parseFloat(b.invests) - parseFloat(a.invests);
              });
              setRanks(animals);
            }
          });
      }
    });
  }
  //下一个
  function nextAnimal() {
    if (selectedItem === animals.length - 1) {
      setSelectedItem(0);
    } else {
      setSelectedItem((x) => x + 1);
    }
  }
  //随机
  function randomAnimal() {
    const random = Math.floor(Math.random() * 16);
    setSelectedItem(random);
  }

  function done(amount) {
    if (!isRealNum(amount)) {
      Toast.info(intl.get("FORMATE_ERROR"));
      return;
    }
    setModalOpen(false);
    setLoading(true);
    handleTransfer(account, tokens[0], amount, selectedItem + 1)
      .then((trx) => {
        setLoading(false);
        Toast.success("success");
        //刷新rank
        getRanks();
      })
      .catch((err) => {
        setLoading(false);
        const errData = JSON.parse(err);
        if (errData.error) {
          const message = errData.error.details[0].message;
          Toast.fail(message);
        }
      });
  }
  return (
    <div className={styles.root}>
      <InputModal
        open={modalOpen}
        onClose={() => setModalOpen(false)}
        animal={animals[selectedItem]}
        nextAnimal={() => nextAnimal()}
        randomAnimal={() => randomAnimal()}
        done={(value) => done(value)}
      />
      <Backdrop
        open={loading}
        style={{ zIndex: 100 }}
        // onClick={() => setLoading(false)}
      >
        <CircularProgress color="inherit" />
      </Backdrop>
      <div className={styles.background}>
        <div className={styles.title}>
          {intl.get("PHASE", { number: period })}
        </div>
        <div className={styles.animalContainer}>
          {animals.map((item, index) => {
            return (
              <div
                className={
                  selectedItem === index
                    ? styles.animalItem320S
                    : styles.animalItem320N
                }
                key={index}
                onClick={() => setSelectedItem(index)}
              >
                <img src={item.default} width={36} height={36} alt="animal" />
              </div>
            );
          })}
        </div>
        <div className={styles.doneRoot}>
          <div
            className="doneBtn"
            onClick={() => {
              if (selectedItem === -1) {
                randomAnimal();
                setModalOpen(true);
                return;
              }
              setModalOpen(true);
            }}
          >
            {intl.get("BET")}
          </div>
        </div>
        <div className={styles.ruleRoot}>
          <div
            className="recordBtn"
            onClick={() =>
              history.push({
                pathname: "/record",
                account: account,
              })
            }
          >
            {intl.get("RECORD")}
          </div>
          <div
            className="ruleBtn"
            onClick={() => {
              // Toast.info(intl.get("NOOPEN"));
              history.push("/rule");
            }}
          >
            {intl.get("RULE")}
          </div>
        </div>
        <div className={styles.listTitle}>{intl.get("RANK")}</div>
        <List className={styles.list}>
          {ranks.map((item, index) => (
            <div key={index} className={styles.listItem}>
              <img
                width={23}
                height={23}
                src={animals[item.number - 1].default}
                alt="animal"
              />
              <div className={styles.itemTitle}>
                {intl.get("RAND_TITLE", { amount: item.invests })}{" "}
              </div>
            </div>
          ))}
        </List>
      </div>
    </div>
  );
}
