package plus.aos.jaos.core.request.chain.json2bin;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * 
 * @author aoscoder (aoscoder@outlook.com)
 */
@Data
@AllArgsConstructor
@NoArgsConstructor
public class BuyRamArg {
    private String payer;
    private String receiver;
    private int bytes;
}
