package plus.aos.jaos.core.response.history.transaction;

import java.util.List;

import lombok.Data;

/**
 * 
 * @author aoscoder (aoscoder@outlook.com)
 */
@Data
public class Trx {

    private String compression;
    private String id;
    private String packedContextFreeData;
    private String packedTrx;
    private List<String> signatures;
    private TransactionItem transaction;
    
}
