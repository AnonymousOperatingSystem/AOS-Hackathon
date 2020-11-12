import React, { Component } from "react";
import { Modal, Flex, Toast } from "antd-mobile";
import "../styles/Modals.css";
import { isEmpty } from "../utils/tools";
class TakeOutModal extends Component {
  constructor(props) {
    super(props);

    this.state = {
      amount: "",
    };
  }

  render() {
    const { visible, closeAction, doneAction, modal } = this.props;
    let symbol = "";
    let balance = "";
    if (modal) {
      const staked = modal.staked;
      symbol = staked.quantity.split(" ")[1];
      balance = staked.quantity;
    }
    const submit = () => {
      const { amount } = this.state;
      if (isEmpty(amount)) {
        Toast.info("cannot be empty");
        return;
      }
      doneAction(amount);
    };
    return (
      <Modal
        visible={visible}
        transparent
        className="modal_warpper"
        onClose={closeAction}
        title="Unstake"
      >
        <Flex direction="column" align="stretch" style={{ marginLeft: 8 }}>
          <div className="success_title">{`Unstake ${symbol} to your current wallet.`}</div>
          <Flex
            justify="between"
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
              value={this.state.amount}
              onChange={(e) => {
                const { value } = e.target;
                this.setState({ amount: value });
              }}
            />
            <div
              style={{ color: "#FF7644" }}
              onClick={() => {
                symbol = balance.split(" ")[0];
                this.setState({
                  amount: symbol,
                });
              }}
            >
              ALL
            </div>
          </Flex>

          <div className="success_message">{`Total staked：${balance}`}</div>
        </Flex>

        <div className="success_confirm" onClick={submit}>
          Unstake
        </div>
      </Modal>
    );
  }
}

export default TakeOutModal;
