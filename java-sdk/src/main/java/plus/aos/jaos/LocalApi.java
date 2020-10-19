package plus.aos.jaos;

import com.fasterxml.jackson.databind.ObjectMapper;
import plus.aos.jaos.core.common.SignArg;
import plus.aos.jaos.core.request.chain.transaction.PushTransactionRequest;
import plus.aos.jaos.impl.AosApiServiceGenerator;

/**
 * Local API with AOS without RPC
 *
 * @author aoscoder (aoscoder@outlook.com)
 */
public interface LocalApi {

    /**
     *
     * 创建私钥
     * create private key
     * @return
     */
    String createPrivateKey();

    /**
     * 根据私钥转成公钥
     * transfer private key to public key
     * @param privateKey 私钥
     * @return 公钥
     */
    String toPublicKey(String privateKey);

    /**
     * 转账
     * transfer
     * @param arg 参数 argument
     * @param privateKey 私钥 private key
     * @param from 转出地址 from address
     * @param to 转入地址 to address
     * @param quantity 数量 quantity
     * @param memo 备注 memo
     * @return
     */
    default PushTransactionRequest transfer(SignArg arg, String privateKey, String from, String to, String quantity, String memo) {
        return transfer(arg, privateKey, "aosio.token", from, to, quantity, memo);
    }

    /**
     * 转帐
     * @param arg 参数 argument
     * @param privateKey 私钥 private key
     * @param account 账户 account
     * @param from  转出地址 from address
     * @param to 转入地址 to address
     * @param quantity 数量 quantity
     * @param memo 备注 memo
     * @return
     */
    PushTransactionRequest transfer(SignArg arg, String privateKey, String account, String from, String to, String quantity, String memo);

    /**
     * 得到对象mapper
     * @return
     */
    default ObjectMapper getObjectMapper() {
        return AosApiServiceGenerator.getMapper();
    }
}
