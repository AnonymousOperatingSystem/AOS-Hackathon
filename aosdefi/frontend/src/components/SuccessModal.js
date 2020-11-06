import React, { Component } from "react";
import { Modal } from "antd-mobile";
import successIcon from "../assets/success_icon@2x.png";
import "../styles/Modals.css";
class SuccessModal extends Component {
  render() {
    const { visible, closeAction, message } = this.props;
    return (
      <Modal
        visible={visible}
        transparent
        className="modal_warpper"
        onClose={closeAction}
        title={
          <img
            src={successIcon}
            width={58}
            height={58}
            style={{ marginTop: 15 }}
            alt="success"
          />
        }
        // wrapProps={{ onTouchStart: ()=> setSuccessModal(false)}}
      >
        <div className="success_title">Successs</div>
        <div className="success_message">{message}</div>
        <div className="success_confirm" onClick={closeAction}>
          Confirm
        </div>
      </Modal>
    );
  }
}

export default SuccessModal;
