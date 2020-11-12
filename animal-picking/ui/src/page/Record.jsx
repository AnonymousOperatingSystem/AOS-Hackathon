import { Backdrop, CircularProgress, List } from "@material-ui/core";
import React, { useEffect, useState } from "react";
import EmptyPage from "../component/EmptyPage";
import styles from "../style/Record.module.scss";
import { getPeriodInfo, getPlayerInfo } from "../service/rpcService";
import { groupBy } from "../utils";
import { Toast } from "antd-mobile";
import { animals } from "../assets/animals/animals";
import { claimRewards } from "../service/aosService";
import { useLocation } from "react-router-dom";
import intl from "react-intl-universal";

let isFirst = true;
function Record() {
  const [loading, setLoading] = useState(true);
  const [isEmpty, setIsEmpty] = useState(false);
  const [rows, setRows] = useState([]);
  const { account = "" } = useLocation();
  useEffect(() => {
    // const account = sessionStorage.getItem("account");
    isFirst = false;
    getPlayerInfo(account.name)
      .then((resp) => {
        setLoading(false);
        const rows = resp.rows;
        if (rows.length === 0) {
          //未参与
          setIsEmpty(true);
          return;
        }

        const pickings = rows[0].pickings;
        const sorted = groupBy(pickings, (item) => [item.period]);
        var s1 = new Set();
        pickings.forEach((item) => {
          s1.add(item.period);
        });
        const periods = Array.from(s1);
        Promise.all(periods.map((period) => getPeriodInfo(period))).then(
          (detail) => {
            let pools = [];
            for (let [index, item] of detail.entries()) {
              const rowData = item.rows[0];
              const search = sorted[index].filter(
                (item) => item.number === rowData.lucky
              );
              //是否刚建
              let isInint = rowData.open === 1 && rowData.lucky === 0;
              pools.push({
                datas: sorted[index],
                period: rowData.period,
                isInint,
                isLucky: search.length !== 0,
              });
            }
            setRows(pools);
          }
        );
      })
      .catch((error) => {
        setLoading(false);
      });
  }, []);

  function submit() {
    setLoading(true);
    claimRewards(account)
      .then((trx) => {
        setLoading(false);
        Toast.success("success");
      })
      .catch((err) => {
        setLoading(false);
        const errData = JSON.parse(err);
        if (errData.error) {
          const message = errData.error.details[0].message;
          Toast.info(message);
        }
      });
  }
  return (
    <div className={styles.root}>
      <Backdrop open={loading} style={{ zIndex: 100 }}>
        <CircularProgress color="inherit" />
      </Backdrop>
      {loading && isFirst ? null : isEmpty ? (
        <EmptyPage />
      ) : (
        <>
          <List className={styles.list}>
            {rows.map((sectionData) => {
              const { datas, period, isInint, isLucky } = sectionData;
              return (
                <div key={period}>
                  <div className={styles.listHeader}>
                    <div>{intl.get("PHASE", { number: period })}</div>
                    <span
                      className={
                        isLucky
                          ? styles.statusTitleLucky
                          : styles.statusTitleNormal
                      }
                    >
                      {isInint
                        ? intl.get("WAIT")
                        : isLucky
                        ? intl.get("WIN")
                        : intl.get("LOSE")}
                    </span>
                  </div>

                  {datas.map((item, index) => (
                    <div key={index} className={styles.listItem}>
                      <img
                        width={23}
                        height={23}
                        src={animals[item.number - 1].default}
                        alt="animal"
                      />
                      <div className={styles.itemTitle}>
                        {intl.get("BETED", { amount: item.invest })}
                      </div>
                    </div>
                  ))}
                </div>
              );
            })}
          </List>
          <div className={styles.claimButton} onClick={() => submit()}>
            {intl.get("CLAIM")}
          </div>
        </>
      )}
    </div>
  );
}

export default Record;
