package plus.aos.jaos.core.response.chain.transaction;

import lombok.Data;

@Data
public class PushedTransaction {

    private Processed processed;
    private String transactionId;

}
