package plus.aos.jaos.impl;

import plus.aos.jaos.LocalApi;
import plus.aos.jaos.convert.Packer;
import plus.aos.jaos.core.common.SignArg;
import plus.aos.jaos.core.common.transaction.PackedTransaction;
import plus.aos.jaos.core.common.transaction.TransactionAction;
import plus.aos.jaos.core.common.transaction.TransactionAuthorization;
import plus.aos.jaos.core.request.chain.json2bin.TransferArg;
import plus.aos.jaos.core.request.chain.transaction.PushTransactionRequest;
import plus.aos.jaos.util.KeyUtil;
import plus.aos.jaos.util.Raw;

import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.List;

/**
 * 
 * @author aoscoder (aoscoder@outlook.com)
 */
public class LocalApiImpl implements LocalApi {

    @Override
    public String createPrivateKey() {
        return KeyUtil.createPrivateKey();
    }

    @Override
    public String toPublicKey(String privateKey) {
        return KeyUtil.toPublicKey(privateKey);
    }

    @Override
    public PushTransactionRequest transfer(SignArg arg, String privateKey, String account, String from, String to, String quantity, String memo) {
        // ① pack transfer data
        TransferArg transferArg = new TransferArg(from, to, quantity, memo);
        String transferData = Packer.packTransfer(transferArg);
        //

        // ③ create the authorization
        List<TransactionAuthorization> authorizations = Arrays.asList(new TransactionAuthorization(from, "active"));

        // ④ build the all actions
        List<TransactionAction> actions = Arrays.asList(//
                new TransactionAction(account, "transfer", authorizations, transferData)//
        );

        // ⑤ build the packed transaction
        PackedTransaction packedTransaction = new PackedTransaction();
        packedTransaction.setExpiration(arg.getHeadBlockTime().plusSeconds(arg.getExpiredSecond()));
        packedTransaction.setRefBlockNum(arg.getLastIrreversibleBlockNum());
        packedTransaction.setRefBlockPrefix(arg.getRefBlockPrefix());
        
        packedTransaction.setMaxNetUsageWords(0);
        packedTransaction.setMaxCpuUsageMs(0);
        packedTransaction.setDelaySec(0);
        packedTransaction.setActions(actions);

        String hash = sign(privateKey, arg, packedTransaction);
        PushTransactionRequest req = new PushTransactionRequest();
        req.setTransaction(packedTransaction);
        req.setSignatures(Arrays.asList(hash));
        return req;
    }

    private String sign(String privateKey, SignArg arg, PackedTransaction t) {

        Raw raw = Packer.packPackedTransaction(arg.getChainId(), t);
        raw.pack(ByteBuffer.allocate(33).array());// TODO: what's this?
        String hash = KeyUtil.signHash(privateKey, raw.bytes());
        return hash;
    }
}
