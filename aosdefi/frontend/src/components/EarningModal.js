import React, { Component } from "react";
import { Modal, Flex } from "antd-mobile";
import "../styles/Modals.css";
class EarningModal extends Component {
  render() {
    const { visible, closeAction, doneAction, modal } = this.props;

    let total = "";
    if (modal) {
      const { rewards } = modal;
      total = rewards.quantity;
    }
    return (
      <Modal
        visible={visible}
        transparent
        className="modal_warpper"
        onClose={closeAction}
        title="Claim"
      >
        <Flex direction="column" align="center" style={{ marginLeft: 8 }}>
          <div className="success_message">
            Claim earnings to your current wallet.
          </div>

          <div
            className="success_title"
            style={{ marginTop: 25, marginBottom: 26 }}
          >
            {total}
          </div>
        </Flex>
        <div className="success_confirm" onClick={doneAction}>
          Confirm
        </div>
      </Modal>
    );
  }
}

export default EarningModal;
