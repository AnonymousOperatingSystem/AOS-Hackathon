import React, { useState } from "react";
import { makeStyles, withStyles } from "@material-ui/core/styles";
import { Slide, Dialog } from "@material-ui/core";
import inputBg from "../assets/inputBg.png";
import intl from "react-intl-universal";

const Transition = React.forwardRef(function Transition(props, ref) {
  return <Slide direction="up" ref={ref} {...props} />;
});
const DialogModal = withStyles({
  paper: {
    backgroundColor: "#FFFFFF",
    borderRadius: 6,
    width: 290,
  },
})(Dialog);

const useStyles = makeStyles({
  container: {
    marginTop: 26,
    color: "#1E1E1E",
    display: "flex",
    height: "100%",
    fontSize: 14,
    flexDirection: "column",
    alignItems: "center",
  },

  animalRoot: {
    marginBottom: 30,
    backgroundImage: `url(${inputBg})`,
    width: 250,
    height: 42,
    backgroundSize: "cover",
    padding: "0px 4px",
    boxSizing: "border-box",
    display: "flex",
    flexDirection: "row",
    justifyContent: "space-between",
    alignItems: "center",
  },
  input: {
    textAlign: "center",
    fontSize: 14,
    width: "100%",
    margin: "0px 15px",
    color: "#666",
  },
  smallBtn: {
    width: 56,
    height: 34,
    backgroundColor: "#FFEDCF",
    borderRadius: 19,
    boxSizing: "border-box",
    textAlign: "center",
    color: "#A13D0D",
    fontSize: 13,
    paddingTop: 10,
    boxShadow: "inset 0 1px 10px 0 rgba(198,110,24,0.55)",
  },
  doneButton: {
    backgroundColor: "#FFEDCF",
    boxShadow: "inset 0 1px 10px 0 rgba(198,110,24,0.55)",
    borderRadius: 20,
    height: 38,
    color: "#A13D0D",
    fontSize: 13,
    width: 250,
    textAlign: "center",
    paddingTop: 12,
    boxSizing: "border-box",
    marginBottom: 34,
  },
});

function InputModal(props) {
  const { onClose, open, animal, nextAnimal, randomAnimal, done } = props;
  const classes = useStyles();
  const [amount, setAmount] = useState("");
  return (
    <DialogModal onClose={onClose} open={open} TransitionComponent={Transition}>
      <div className={classes.container}>
        <div className={classes.animalRoot} style={{ marginBottom: 22 }}>
          <input
            className={classes.input}
            placeholder={intl.get("INPUT_AMOUNT")}
            type="number"
            onChange={(e) => setAmount(e.target.value)}
          />
        </div>
        <div className={classes.animalRoot}>
          <div className={classes.smallBtn} onClick={() => randomAnimal()}>
            {intl.get("RANDOM")}
          </div>
          <img
            width={41}
            height={49}
            src={animal ? animal.default : ""}
            alt="animal"
          />
          <div className={classes.smallBtn} onClick={() => nextAnimal()}>
            {intl.get("NEXT")}
          </div>
        </div>
        <div className={classes.doneButton} onClick={() => done(amount)}>
          {intl.get("DONE_BET")}
        </div>
      </div>
    </DialogModal>
  );
}

export default InputModal;
