/* eslint-disable react-hooks/exhaustive-deps */
import React, { useEffect, useState } from "react";
import { Flex, ListView, ActivityIndicator, Toast } from "antd-mobile";
import DepositModal from "./DepositModal";
import TakeOutModal from "./TakeOutModal";
import SuccessModal from "./SuccessModal";
import StakeModal from "./StakeModal";
import {
  getBlockInfo,
  getCurrentBalance,
  getPoolDetail,
  getPools,
  getUserInfo,
} from "../api/rpcService";
import { createPool, withdraw } from "../api/aosService";
import { computerAPY } from "../utils/tools";

function Home(props) {
  const [dataSource, setDataSource] = useState(
    new ListView.DataSource({
      rowHasChanged: (row1, row2) => row1 !== row2,
    })
  );

  const [successModal, setSuccessModal] = useState({
    visible: false,
    message: "",
  });
  const [depositModal, setDepositModal] = useState({
    visible: false,
    modal: null,
  });
  const [stakeModal, setStakeModal] = useState({
    visible: false,
    modal: null,
  });
  const [takeoutModal, setTakeoutModal] = useState({
    visible: false,
    modal: null,
  });

  const [userCount, setUserCount] = useState(0);
  const [reload, setReload] = useState(false);
  useEffect(() => {
    setAnimating(true);
    getBlockInfo().then((info) => {
      const headDate = info.head_block_time;
      getPools().then((resp) => {
        const rows = resp.rows;
        if (rows.length > 0) {
          setUserCount(rows[0].users);
          let datas = [];
          if (rows.length > 0) {
            datas = rows[0].pairs;
          } else {
            setAnimating(false);
          }
          Promise.all(datas.map((item) => getPoolDetail(item.id))).then(
            (detail) => {
              let pools = [];
              for (let [index, item] of detail.entries()) {
                const rowData = item.rows[0];
                const {
                  init_supply,
                  start_time,
                  fade_factor,
                  token_staked,
                } = rowData;
                const quantity = init_supply.quantity.split(" ")[0];
                const staked = token_staked[token_staked.length - 1].staked;
                const stakeQuantity = staked.quantity.split(" ")[0];
                // const stakePercent = 1 / (1 + parseInt(stakeQuantity))

                // const fixed16 = x.toFixed(16);
                const apy = computerAPY(
                  headDate,
                  start_time,
                  stakeQuantity,
                  quantity,
                  fade_factor
                );
                pools.push({ ...item.rows[0], ...datas[index], apy });
              }
              const newData = dataSource;
              setAnimating(false);
              setDataSource(newData.cloneWithRows(pools));
            }
          );
        } else {
          setAnimating(false);
        }
      });
    });
  }, [props.reload, reload]);

  const [animating, setAnimating] = useState(false);
  const row = (rowData, rowID) => {
    const { total_supply, token_staked } = rowData;
    let supplySymbol = total_supply.quantity.split(" ")[1];
    let staked = token_staked[token_staked.length - 1].staked;
    let stakeSymbol = staked.quantity.split(" ")[1];

    return (
      <div key={rowID} className="row-container">
        <Flex direction="column" align="stretch">
          <div className="h_r_title">{`${stakeSymbol}/${supplySymbol} Pool(Stake ${`${stakeSymbol}`} to claim ${supplySymbol})`}</div>
          <div className="h_r_amount">{staked.quantity}</div>
          <Flex justify="between" className="h_r_stake">
            <div>Staked</div>
            <div>APY {rowData.apy}%</div>
          </Flex>
          <Flex justify="center" style={{ marginTop: 15 }}>
            <div
              className="h_r_Deposit"
              onClick={() => {
                //获取余额
                setAnimating(true);
                getCurrentBalance(
                  props.account.name,
                  staked.contract,
                  stakeSymbol
                )
                  .then((balance) => {
                    // setAnimating(false)
                    //TODO: 判断是否是第一次存入，是否在100以内
                    getUserInfo(props.account.name).then((resp) => {
                      setAnimating(false);
                      if (resp.rows.length === 0) {
                        //没有存过
                        setStakeModal({
                          count: userCount,
                          modal: { ...rowData, balance },
                          visible: true,
                        });
                      } else {
                        setDepositModal({
                          modal: { ...rowData, balance },
                          visible: true,
                        });
                      }
                    });
                  })
                  .catch(() => setAnimating(false));
              }}
            >
              Stake
            </div>
            <div
              className="h_r_TakeOut"
              onClick={() => {
                setAnimating(true);
                getUserInfo(props.account.name).then((resp) => {
                  setAnimating(false);
                  const stakes = resp.rows[0].stakes;
                  const selected = stakes.filter(
                    (item) => item.pool_id === rowData.id
                  );
                  if (selected.length > 0) {
                    setTakeoutModal({
                      modal: selected[0],
                      visible: true,
                    });
                  } else {
                    Toast.info("There is no pledge");
                  }
                });
              }}
            >
              Unstake
            </div>
          </Flex>
        </Flex>
      </div>
    );
  };
  return (
    <div style={{ display: "flex", flexDirection: "column", height: "100%" }}>
      {depositModal.visible && (
        <DepositModal
          visible={depositModal.visible}
          state={{ amount: "", percent: "--" }}
          modal={depositModal.modal}
          closeAction={() =>
            setDepositModal({ ...depositModal, visible: false })
          }
          doneAction={(amount) => {
            const modal = depositModal.modal;
            const memo = `STAKE:${modal.id}|`;
            const { token_staked } = modal;
            let staked = token_staked[token_staked.length - 1].staked;
            let stakeSymbol = staked.quantity.split(" ")[1];

            setAnimating(true);

            createPool(
              props.account,
              staked.contract,
              stakeSymbol,
              amount,
              memo
            )
              .then((trx) => {
                //进入结果页面
                setAnimating(false);

                setDepositModal({ ...successModal, visible: false });
                setTimeout(() => {
                  setSuccessModal({
                    visible: true,
                    message: "Transaction has been made.",
                  });
                  setReload(!reload);
                }, 250);
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
      {stakeModal.visible && (
        <StakeModal
          visible={stakeModal.visible}
          modal={stakeModal.modal}
          count={stakeModal.count}
          closeAction={() => setStakeModal({ ...stakeModal, visible: false })}
          doneAction={(amount, inviteCode) => {
            const modal = stakeModal.modal;
            const memo = `STAKE:${modal.id}|${inviteCode}`;
            const { token_staked } = modal;
            let staked = token_staked[token_staked.length - 1].staked;
            let stakeSymbol = staked.quantity.split(" ")[1];

            setAnimating(true);
            createPool(
              props.account,
              staked.contract,
              stakeSymbol,
              amount,
              memo
            )
              .then((trx) => {
                //进入结果页面
                setAnimating(false);

                setStakeModal({ ...stakeModal, visible: false });
                setTimeout(() => {
                  setSuccessModal({
                    visible: true,
                    message: "Transaction has been made.",
                  });
                  setReload(!reload);
                }, 250);
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

      {takeoutModal.visible && (
        <TakeOutModal
          visible={takeoutModal.visible}
          modal={takeoutModal.modal}
          closeAction={() =>
            setTakeoutModal({ ...takeoutModal, visible: false })
          }
          doneAction={(amount) => {
            const modal = takeoutModal.modal;

            const staked = modal.staked;
            // return
            const contractName = staked.contract;
            const symbol = staked.quantity.split(" ")[1];
            setAnimating(true);
            withdraw(props.account, modal.pool_id, contractName, amount, symbol)
              .then((trx) => {
                //进入结果页面
                setAnimating(false);

                setTakeoutModal({ ...takeoutModal, visible: false });
                setTimeout(() => {
                  setSuccessModal({
                    visible: true,
                    message: `${amount} ${symbol} has been claimed to wallet.`,
                  });
                  setReload(!reload);
                }, 250);
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
      <SuccessModal
        visible={successModal.visible}
        message={successModal.message}
        closeAction={() => setSuccessModal({ ...successModal, visible: false })}
      />
      <ListView
        dataSource={dataSource}
        renderHeader={() => <div style={{ height: 38 }}></div>}
        renderRow={row}
        className="am-list"
        sectionBodyClassName="am-section-body"
        pageSize={4}
        useBodyScroll
      />
    </div>
  );
}

export default Home;
