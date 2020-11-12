import ScatterJS from "@scatterjs/core";
import ScatterEOS from "@scatterjs/eosjs";
import Eos from "eosjs";

import {
  scatterNetwork,
  DAPP_NAME,
  ASSETS_CONTRACT,
  RECEIVE_ACCOUNT,
} from "./config";

ScatterJS.plugins(new ScatterEOS());

async function login() {
  const connected = await ScatterJS.scatter.connect(DAPP_NAME);
  if (!connected) {
    return false;
  }
  const scatter = ScatterJS.scatter;
  const requiredFields = { accounts: [scatterNetwork] };
  await scatter.getIdentity(requiredFields);
  return await scatter.identity.accounts.find(
    (x: any) => x.blockchain === "eos"
  );
}
async function logout() {
  return await ScatterJS.scatter.logout();
}

async function createPool(
  account: any,
  token1: string,
  symbol: string,
  amount: string,
  memo: string
) {
  const amountFloat = parseFloat(amount).toFixed(4);
  const quantity = `${amountFloat} ${symbol}`;
  const action = {
    account: token1,
    name: token1 === RECEIVE_ACCOUNT ? "transferpp" : "transfer",
    authorization: [
      {
        actor: account.name,
        permission: account.authority,
      },
    ],
    data: {
      from: account.name, // 创建者账户
      to: ASSETS_CONTRACT, // 本合约账户
      quantity: quantity, // 矿池token总金额
      memo, // 格式是：CREATE:抵押token的合约|抵押token的symbol(包含精度，如:4,AOS)|衰减因子|总挖矿天数
    },
  };
  return await pushTransaction(action);
}

//领取收益
async function earning(account: any, pid: string) {
  const action = {
    account: ASSETS_CONTRACT,
    name: "claim",
    authorization: [
      {
        actor: account.name,
        permission: account.authority,
      },
    ],
    data: {
      player: account.name, // 创建者账户
      pid, // 本合约账户
    },
  };
  return await pushTransaction(action);
}

//提出
async function withdraw(
  account: any,
  pid: string,
  tokenContract: string,
  amount: string,
  symbol: string
) {
  const amountFloat = parseFloat(amount).toFixed(4);
  const quantity = `${amountFloat} ${symbol}`;
  const action = {
    account: ASSETS_CONTRACT,
    name: "withdraw",
    authorization: [
      {
        actor: account.name,
        permission: account.authority,
      },
    ],
    data: {
      player: account.name, // 创建者账户
      pid, // 本合约账户
      token_contract: tokenContract,
      token_quantity: quantity,
    },
  };
  return await pushTransaction(action);
}
async function pushTransaction(action: any) {
  const scatter = ScatterJS.scatter;
  const eosOptions = { expireInSeconds: 60 };
  const eos = scatter.eos(scatterNetwork, Eos, eosOptions);
  return await eos.transaction(
    {
      actions: [action],
    },
    {
      blocksBehind: 3,
      expireSeconds: 60,
    }
  );
}
export { login, logout, createPool, earning, withdraw };
