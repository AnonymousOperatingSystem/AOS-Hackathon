package plus.aos.jaos.core.response.chain;

import java.time.LocalDateTime;

import plus.aos.jaos.core.response.block.NewProducer;
import plus.aos.jaos.core.response.history.transaction.Transaction;
import lombok.Data;

@Data
public class Block {

    private String actionMroot;
    private Long blockNum;
    private Long confirmed;
    private String id;
    private String previous;
    private String producer;
    private String producerSignature;
    private Long refBlockPrefix;
    private Long scheduleVersion;
    private LocalDateTime timestamp;
    private String transactionMroot;

    private String actionMerkleRoot;

    private String blockMerkleRoot;

    private NewProducer newProducers;

    private Transaction[] transactions;

    private Object[] headerExtensions;

    private Object[] blockExtensions;
}
