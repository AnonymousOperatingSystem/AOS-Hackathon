import React from "react";
import intl from "react-intl-universal";

function Rule() {
  return (
    <>
      <h2 style={{ color: "white", textAlign: "center" }}>
        {intl.get("GAMERULE")}{" "}
      </h2>
      <p
        style={{
          color: "white",
          textAlign: "center",
          margin: "20px 20px",
          lineHeight: "2em",
        }}
      >
        {intl.get("RULEDETAIL")}
      </p>
    </>
  );
}

export default Rule;
