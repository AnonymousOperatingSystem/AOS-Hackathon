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
public class DelegatebwArg {

    private String from;
    private String receiver;
    private String stakeNetQuantity;
    private String stakeCpuQuantity;
    // Transfer voting power and right to unstake AOS to receiver
    // 0 or 1
    private Long transfer = 0L;
}
