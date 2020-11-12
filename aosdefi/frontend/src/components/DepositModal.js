import React, { useState } from "react";
import { Modal, Flex, Toast } from "antd-mobile";
import "../styles/Modals.css";
import { isEmpty, percentNum } from "../utils/tools";

function DepositModal(props) {
  const [amount, setAmount] = useState("");
  const [percent, setPercent] = useState("--");

  const { visible, closeAction, doneAction, modal } = props;

  function submit() {
    if (isEmpty(amount)) {
      Toast.info("cannot be empty");
      return;
    }
    doneAction(amount);
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
            placeholder=""
            value={amount}
            type="number"
            onChange={(e) => {
              const { value } = e.target;
              setAmount(value);
              const { token_staked } = modal;
              let staked = token_staked[token_staked.length - 1].staked;
              let stakeCount = staked.quantity.split(" ")[0];
              let percent = percentNum(
                parseFloat(value),
                parseFloat(stakeCount) + parseFloat(value)
              );
              setPercent(percent);
            }}
          />
        </Flex>

        <div className="success_message">{`Balance：${
          isEmpty(modal.balance) ? "0" : modal.balance
        }`}</div>
        <div className="success_message">Estimated proportion: {percent}</div>
      </Flex>

      <div className="success_confirm" onClick={submit}>
        Confirm
      </div>
    </Modal>
  );
}

export default DepositModal;
