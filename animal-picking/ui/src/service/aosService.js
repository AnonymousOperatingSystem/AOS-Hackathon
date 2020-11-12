import ScatterJS from "@scatterjs/core";
import ScatterEOS from "@scatterjs/eosjs";
import Eos from "eosjs";

import { scatterNetwork, DAPP_NAME, ASSETS_CONTRACT } from "./config";

ScatterJS.plugins(new ScatterEOS());

async function login() {
  const connected = await ScatterJS.scatter.connect(DAPP_NAME);
  if (!connected) {
    return false;
  }
  const scatter = ScatterJS.scatter;
  const requiredFields = { accounts: [scatterNetwork] };
  await scatter.getIdentity(requiredFields);
  return await scatter.identity.accounts.find((x) => x.blockchain === "eos");
}
async function logout() {
  return await ScatterJS.scatter.logout();
}

async function claimRewards(account) {
  const scatter = ScatterJS.scatter;
  const eosOptions = { expireInSeconds: 60 };
  const eos = scatter.eos(scatterNetwork, Eos, eosOptions);

  return await eos.transaction(
    {
      actions: [
        {
          account: ASSETS_CONTRACT,
          name: "claim",
          authorization: [
            {
              actor: account.name,
              permission: "active",
            },
          ],
          data: {
            user: account.name,
          },
        },
      ],
    },
    {
      blocksBehind: 3,
      expireSeconds: 60,
    }
  );
}
async function handleTransfer(account, token, amount, number) {
  const scatter = ScatterJS.scatter;
  const eosOptions = { expireInSeconds: 60 };
  const eos = scatter.eos(scatterNetwork, Eos, eosOptions);

  // const transactionOptions = { authorization:[`${account.name}@${account.authority}`]};
  const amountFloat = parseFloat(amount).toFixed(4);
  const quantity = `${amountFloat} ${token.symbol}`;
  // return  await eos.transfer(account.name, to, quantity, memo, transactionOptions)
  console.log(account, token, amount, number);
  return await eos.transaction(
    {
      actions: [
        {
          account: token.contract,
          name: token.contract === "ciphertokens" ? "transferpp" : "transfer",
          authorization: [
            {
              actor: account.name,
              permission: account.authority,
            },
          ],
          data: {
            from: account.name,
            to: ASSETS_CONTRACT,
            quantity: quantity,
            memo: `PICK:${number}`,
          },
        },
      ],
    },
    {
      blocksBehind: 3,
      expireSeconds: 60,
    }
  );
}
export { login, logout, handleTransfer, claimRewards };
