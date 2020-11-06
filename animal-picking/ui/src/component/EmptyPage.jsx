import { makeStyles, createStyles } from "@material-ui/core/styles";
import { useHistory } from "react-router-dom";
import emptyIcon from "../assets/empty.png";
import intl from "react-intl-universal";
const useStyles = makeStyles(() =>
  createStyles({
    desc: {
      marginTop: 10,
      fontSize: 13,
      color: "#000000",
      opacity: 0.35,
    },
    done: {
      backgroundColor: "#FFEDCF",
      color: "#A13D0D",
      fontSize: 13,
      borderRadius: 17,
      boxShadow: "inset 0 1px 10px 0 rgba(198,110,24,0.55)",
      width: 192,
      height: 38,
      paddingTop: 9,
      boxSizing: "border-box",
      margin: "auto",
      marginTop: 18,
      textAlign: "center",
    },
  })
);

function EmptyPage() {
  const classes = useStyles();
  const history = useHistory();
  return (
    <div>
      <img width={78} height={80} alt="empty" src={emptyIcon} />
      <div className={classes.desc}>{intl.get("NO_RECORD")}</div>
      <div className={classes.done} onClick={() => history.goBack()}>
        {intl.get("GOBET")}
      </div>
    </div>
  );
}

export default EmptyPage;
