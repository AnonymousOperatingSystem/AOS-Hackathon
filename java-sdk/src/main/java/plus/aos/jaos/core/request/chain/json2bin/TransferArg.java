package plus.aos.jaos.core.request.chain.json2bin;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * 
 * @author aoscoder (aoscoder@outlook.com)
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
public class TransferArg {
    private String from;
    private String to;
    private String quantity;
    private String memo;
}
