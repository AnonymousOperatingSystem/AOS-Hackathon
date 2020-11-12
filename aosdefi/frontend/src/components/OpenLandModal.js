import React, { Component } from "react";
import { Modal, Flex, Toast } from "antd-mobile";
import "../styles/Modals.css";
import { createForm } from "rc-form";
import { isEmpty } from "../utils/tools";
class OpenLandModal extends Component {
  render() {
    const { visible, closeAction, doneAction } = this.props;
    const { getFieldProps } = this.props.form;

    const submit = () => {
      this.props.form.validateFields((error, value) => {
        const {
          depositContract,
          amount,
          deposit,
          factor,
          mining,
          miningContract,
          day,
        } = value;
        if (isEmpty(deposit)) {
          Toast.info("Please enter Deposit TOKEN");
          return;
        }
        if (isEmpty(depositContract)) {
          Toast.info("Please enter Contract address");
          return;
        }
        if (isEmpty(mining)) {
          Toast.info("Please enter Mining TOKEN");
          return;
        }
        if (isEmpty(amount)) {
          Toast.info("Please enter Deposit amount");
          return;
        }
        if (isEmpty(miningContract)) {
          Toast.info("Please enter Contract address");
          return;
        }
        if (isEmpty(factor)) {
          Toast.info("Please enter Attenuation factor");
          return;
        }
        if (isEmpty(day)) {
          Toast.info("Please enter Mining pool validity period");
          return;
        }
        doneAction(value);
      });
    };

    return (
      <Modal
        visible={visible}
        transparent
        className="modal_warpper"
        onClose={closeAction}
        title={false}
      >
        <Flex direction="column" align="stretch" style={{ marginLeft: 8 }}>
          <div className="openLand_title">Deposit TOKEN</div>
          <input {...getFieldProps("deposit")} className="openLand_input" />

          <div className="openLand_title">Contract address</div>
          <input
            {...getFieldProps("depositContract")}
            className="openLand_input"
          />

          <div className="openLand_title">Mining TOKEN</div>
          <input {...getFieldProps("mining")} className="openLand_input" />

          <div className="openLand_title">Deposit amount</div>
          <input {...getFieldProps("amount")} className="openLand_input" />

          <div className="openLand_title">Contract address</div>
          <input
            {...getFieldProps("miningContract")}
            className="openLand_input"
          />

          <div className="openLand_title">Attenuation factor(%)</div>
          <input {...getFieldProps("factor")} className="openLand_input" />

          <div className="openLand_title">Mining pool validity period</div>
          <input
            {...getFieldProps("day")}
            type="number"
            className="openLand_input"
          />
        </Flex>

        <div className="success_confirm" onClick={doneAction} onClick={submit}>
          Confirm
        </div>
      </Modal>
    );
  }
}
const FormOpenLandModal = createForm()(OpenLandModal);
export default FormOpenLandModal;
