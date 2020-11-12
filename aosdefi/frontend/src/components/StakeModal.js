import React, { Component } from "react";
import { Modal, Flex, Toast } from "antd-mobile";
import "../styles/Modals.css";
import { isEmpty, percentNum } from "../utils/tools";
import { VIP_NUMBER } from "../api/config";
class StakeModal extends Component {
  constructor(props) {
    super(props);

    this.state = {
      amount: "",
      inviteCode: "",
    };
  }

  render() {
    const { visible, closeAction, doneAction, count, modal } = this.props;
    let balance = "";
    if (modal) {
      balance = modal.balance;
    }
    const submit = () => {
      const { amount, inviteCode } = this.state;
      if (isEmpty(amount)) {
        Toast.info("cannot be empty");
        return;
      }
      if (count > VIP_NUMBER && isEmpty(inviteCode)) {
        Toast.info("cannot be empty");
        return;
      }
      doneAction(amount, inviteCode);
    };
    let inviteView;
    console.log(count);
    if (count >= VIP_NUMBER) {
      inviteView = (
        <Flex
          style={{
            margin: "6px 0px",
            padding: "0px 15px ",
            height: 45,
            backgroundColor: "#ECECEC",
            borderRadius: 4,
          }}
        >
          <input
            placeholder="enter"
            onChange={(e) => {
              const { value } = e.target;
              this.setState({ inviteCode: value });
            }}
          />
        </Flex>
      );
    } else {
      inviteView = (
        <Flex direction="column" justify="start" align="stretch">
          <Flex
            style={{
              margin: "6px 0px",
              padding: "0px 15px ",
              height: 45,
              backgroundColor: "#ECECEC",
              borderRadius: 4,
            }}
            justify="center"
            align="center"
          >
            <div>No Need</div>
          </Flex>
          <div style={{ color: "#FF7644 " }}>
            Congrats! This is a Level 0 account
          </div>
        </Flex>
      );
    }
    return (
      <Modal
        visible={visible}
        transparent
        className="modal_warpper"
        onClose={closeAction}
        title="Stake"
      >
        <Flex direction="column" align="stretch" style={{ marginLeft: 8 }}>
          <div className="success_title">Enter stake amount</div>
          <Flex
            style={{
              margin: "10px 0px",
              padding: "0px 15px ",
              height: 45,
              backgroundColor: "#ECECEC",
              borderRadius: 4,
            }}
          >
            <input
              placeholder="enter"
              onChange={(e) => {
                const { value } = e.target;
                this.setState({ amount: value });
                const { token_staked } = modal;
                let staked = token_staked[token_staked.length - 1].staked;
                let stakeCount = staked.quantity.split(" ")[0];

                let percent = percentNum(
                  parseFloat(value),
                  parseFloat(stakeCount) + parseFloat(value)
                );
                this.setState({ percent });
              }}
            />
          </Flex>

          <div className="success_message">{`Balance: ${
            isEmpty(balance) ? "0" : balance
          }`}</div>
          <div className="success_message">
            Estimated proportion: {this.state.percent}
          </div>

          <div className="success_title" style={{ marginTop: 15 }}>
            Invitation Code
          </div>
          {inviteView}
        </Flex>

        <div className="success_confirm" onClick={submit}>
          Confirm
        </div>
      </Modal>
    );
  }
}

export default StakeModal;
