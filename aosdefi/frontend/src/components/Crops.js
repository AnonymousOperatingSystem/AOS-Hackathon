/* eslint-disable react-hooks/exhaustive-deps */
import React, { useEffect, useState } from "react";
import { ActivityIndicator, Flex, ListView, Toast } from "antd-mobile";
import SuccessModal from "./SuccessModal";
import { earning } from "../api/aosService";
import { getUserInfo } from "../api/rpcService";
import { formatDate, isEmpty } from "../utils/tools";

function Crops(props) {
  const [dataSource, setDataSource] = useState(
    new ListView.DataSource({
      rowHasChanged: (row1, row2) => row1 !== row2,
    }).cloneWithRows([])
  );

  const [animating, setAnimating] = useState(false);
  const [successModal, setSuccessModal] = useState({
    visible: false,
    message: "",
  });
  // const [earnVisible, setEarnVisible] = useState({
  //   visible:false,
  //   item: null
  // })
  const [reload, setReload] = useState();
  useEffect(() => {
    if (!isEmpty(props.account)) {
      setAnimating(true);
      getUserInfo(props.account.name).then((resp) => {
        setAnimating(false);
        let rows = [];
        if (resp.rows.length > 0) {
          rows = resp.rows[0].stakes;
        }
        setDataSource(dataSource.cloneWithRows(rows));
      });
    }
  }, [reload]);

  const row = (rowData, rowID) => {
    const { rewards, staked, claim_time } = rowData;
    let supplySymbol = rewards.quantity.split(" ")[1];
    let stakeSymbol = staked.quantity.split(" ")[1];
    const totalReward = parseFloat(rewards.quantity.split(" ")[0]);
    const lastDate = formatDate(claim_time * 1000);

    return (
      <div key={rowID} className="row-container">
        <Flex direction="column" align="center">
          <div className="h_r_poor">{`${stakeSymbol}/${supplySymbol} Pool`}</div>
          <div style={{ marginTop: 15 }} className="h_r_title">
            Staked：<span className="h_r_amount">{staked.quantity}</span>
          </div>

          <Flex justify="center" style={{ width: "100", marginTop: 15 }}>
            <div
              className="c_r_out"
              onClick={() => {
                //TODO: 领取每日收益
                setAnimating(true);
                earning(props.account, rowData.pool_id)
                  .then((trx) => {
                    //进入结果页面
                    setAnimating(false);

                    // setEarnVisible({ ...earnVisible,visible:false,});
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
            >
              Claim
            </div>
          </Flex>

          <div className="h_r_title" style={{ marginTop: 8 }}>
            Please collect within two days, otherwise the crops will wither
          </div>

          <div className="h_r_title" style={{ marginTop: 15 }}>
            Total claimed：{rewards.quantity}
          </div>
          {totalReward > 0 && (
            <div style={{ marginTop: 8, color: "#333333" }}>
              Last claim time {lastDate}
            </div>
          )}
        </Flex>
      </div>
    );
  };
  let renderList = (
    <ListView
      dataSource={dataSource}
      renderHeader={() => <div style={{ height: 38 }}></div>}
      renderRow={row}
      className="am-list"
      sectionBodyClassName="am-section-body"
      pageSize={4}
      useBodyScroll
    />
  );
  if (dataSource._cachedRowCount === 0) {
    renderList = (
      <Flex
        style={{ height: 300, backgroundColor: "transparent", color: "white" }}
        justify="center"
        direction="column"
      >
        You haven't grew and crops yet.
      </Flex>
    );
  }

  return (
    <div style={{ display: "flex", flexDirection: "column", flex: 1 }}>
      {/* <EarningModal visible={earnVisible.visible} modal={earnVisible.item}  closeAction={()=> setEarnVisible({...earnVisible,visible:false})} doneAction={() => {
        const modal = earnVisible.item
        //TODO: 领取每日收益  
        setAnimating(true)
        earning(props.account,modal.pool_id).then((trx) => {
          //进入结果页面
          setAnimating(false);
          
          setEarnVisible({ ...earnVisible,visible:false,});
          setTimeout(() => {
            setSuccessModal({
              visible:true,
              message:'Transaction has been made.'
            })
            setReload(!reload)
          }, 250);
          
        })
        .catch((err) => {
          setAnimating(false);
          const errData = JSON.parse(err);
                  const { code } = errData.error;
                  getCodeMessage(code).then((resp) => {
                    if (resp.ok === true) {
                      const data = resp.data;
                      Toast.info(data.data.message, 3);
                    } else {
                      Toast.info(errData.message);
                    }
                  });
        });
      }}/> */}
      <SuccessModal
        visible={successModal.visible}
        message={successModal.message}
        closeAction={() => setSuccessModal({ ...successModal, visible: false })}
      />
      {/* <img src={MiddleBackground} width="100%" alt='middle'/> */}

      <ActivityIndicator toast text="loading" animating={animating} />

      {renderList}
    </div>
  );
}

export default Crops;
