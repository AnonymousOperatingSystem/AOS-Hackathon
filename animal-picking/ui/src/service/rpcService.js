import Eos from "eosjs";
import { ASSETS_CONTRACT, RPCBASE, scatterNetwork } from "./config";

const config = {
  httpEndpoint: RPCBASE,
  chainId: scatterNetwork.chainId, // 通过cleos get info可以获取chainId
  expireInSeconds: 60,
};

const eos = Eos(config);

async function getCurrentBalance(account, contract, symbol) {
  return await eos.getCurrencyBalance(contract, account, symbol);
}

async function getBlockInfo() {
  return await eos.getInfo({});
}

//获取总信息
async function getGlobalInfo() {
  const data = {
    code: ASSETS_CONTRACT, // 本合约账户
    json: true,
    limit: -1,
    lower_bound: "",
    scope: ASSETS_CONTRACT, // 本合约账户
    table: "globals", // 固定写死
    table_key: "",
    upper_bound: "",
  };
  return await eos.getTableRows(data);
}
//获取某一期的情况
async function getPeriodInfo(scope) {
  const data = {
    code: ASSETS_CONTRACT, // 本合约账户
    json: true,
    limit: -1,
    lower_bound: "",
    scope: scope, // 期数
    table: "rounds", // 固定写死
    table_key: "",
    upper_bound: "",
  };
  return await eos.getTableRows(data);
}

//获取某个用户的信息
async function getPlayerInfo(account) {
  const data = {
    code: ASSETS_CONTRACT, // 本合约账户
    json: true,
    limit: -1,
    lower_bound: "",
    scope: account, // 用户名
    table: "players", // 固定写死
    table_key: "",
    upper_bound: "",
  };
  return await eos.getTableRows(data);
}
export {
  getCurrentBalance,
  getBlockInfo,
  getPeriodInfo,
  getPlayerInfo,
  getGlobalInfo,
};
