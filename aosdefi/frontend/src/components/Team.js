/* eslint-disable react-hooks/exhaustive-deps */
import React, { useEffect, useState } from "react";
import { ActivityIndicator, Flex, ListView, Toast } from "antd-mobile";
import copy from "copy-to-clipboard";
import "../styles/Team.css";
import { getUserInfo } from "../api/rpcService";

function Team(props) {
  const [dataSource, setDataSource] = useState(
    new ListView.DataSource({
      rowHasChanged: (row1, row2) => row1 !== row2,
    }).cloneWithRows([])
  );
  const [animating, setAnimating] = useState(false);
  const [inviteCount, setInviteCount] = useState(0);
  const [inviteRewards, setInviteRewards] = useState([]);
  useEffect(() => {
    if (props.account) {
      setAnimating(true);
      getUserInfo(props.account.name).then((resp) => {
        setAnimating(false);
        const rows = resp.rows;
        if (rows.length === 0) {
          setInviteCount(0);
        } else {
          const invitees = rows[0].invitees;
          setInviteCount(invitees.length);
          if (invitees.length !== 0) {
            const tempStakes = [];
            const stakes = rows[0].stakes;
            for (let stake of stakes) {
              const { invite_rewards, invite_staked } = stake;
              const left = invite_staked.quantity.split(" ")[0];
              const right = invite_rewards.quantity.split(" ")[0];
              if (parseFloat(left) > 0 || parseFloat(right) > 0) {
                tempStakes.push(stake);
              }
            }

            setInviteRewards(tempStakes);

            //遍历请求被邀请人
            Promise.all(invitees.map((item) => getUserInfo(item))).then(
              (detail) => {
                let pools = [];
                for (let item of detail) {
                  pools.push(item.rows[0]);
                }
                const newData = dataSource;
                setAnimating(false);
                setDataSource(newData.cloneWithRows(pools));
              }
            );
          }
        }
      });
    }
  }, []);
  const row = (rowData, rowID) => {
    const totalStakes = rowData.stakes;
    return (
      <div key={rowID} className="t_container">
        <Flex direction="column" align="stretch">
          <div className="t_r_title">{rowData.id}</div>

          {totalStakes.map((item, index) => {
            const { rewards, staked } = item;
            const left = staked.quantity.split(" ");
            const right = rewards.quantity.split(" ");
            return (
              <Flex justify="between" style={{ marginTop: 5 }} key={index}>
                <div className="t_invite_left">
                  {left[1]}/{right[1]}
                </div>
                <div className="t_invite_right">staked {left}</div>
              </Flex>
            );
          })}

          <div className="t_line" />
        </Flex>
      </div>
    );
  };

  return (
    <div>
      {/* <img src={TopBackground} width="100%" /> */}

      <ActivityIndicator toast text="loading" animating={animating} />

      <ListView
        dataSource={dataSource}
        renderHeader={() => (
          <div style={{ padding: 0, margin: 0 }}>
            <Flex direction="column" align="stretch">
              <Flex
                style={{
                  backgroundColor: "white",
                  padding: "0px 18px",
                  height: 48,
                  borderRadius: 10,
                }}
                justify="between"
                align="center"
              >
                <div className="t_r_title" style={{ margin: 0 }}>
                  Me: {props.account ? props.account.name : ""}
                </div>
                <div
                  className="t_r_right"
                  onClick={() => {
                    // 复制
                    const account = props.account ? props.account.name : "";
                    copy(account);
                    Toast.success("copy success");
                  }}
                >
                  Copy
                </div>
              </Flex>
              <div
                style={{
                  marginTop: 15,
                  marginBottom: 10,
                  color: "white",
                  fontWeight: 500,
                }}
              >
                People I invited（{inviteCount}）
              </div>
              {inviteRewards.length > 0 && (
                <div
                  style={{
                    padding: "0px 20px 17px",
                    marginBottom: 10,
                    borderRadius: 10,
                    backgroundColor: "white",
                  }}
                >
                  {inviteRewards.map((item, index) => {
                    const { invite_rewards, invite_staked } = item;
                    const left = invite_staked.quantity.split(" ");
                    const right = invite_rewards.quantity.split(" ");
                    return (
                      <Flex
                        direction="row"
                        justify="between"
                        style={{ marginTop: 15 }}
                        key={index}
                      >
                        <div className="t_r_left">
                          {left[1]}/{right[1]} Pool
                        </div>
                        <div className="t_r_right">+ {right}</div>
                      </Flex>
                    );
                  })}
                </div>
              )}
            </Flex>
          </div>
        )}
        renderRow={row}
        className="am-list"
        sectionBodyClassName="t-section-body"
        pageSize={4}
        useBodyScroll
      />
    </div>
  );
}

export default Team;
