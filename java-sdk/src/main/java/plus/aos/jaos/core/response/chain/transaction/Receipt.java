package plus.aos.jaos.core.response.chain.transaction;

import lombok.Data;

/**
 * 
 * @author aoscoder (aoscoder@outlook.com)
 */
@Data
public class Receipt {

    private Long cpuUsageUs;
    private Integer netUsageWords;
    private String status;
}
