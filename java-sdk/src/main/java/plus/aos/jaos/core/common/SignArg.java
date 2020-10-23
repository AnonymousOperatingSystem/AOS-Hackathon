package plus.aos.jaos.core.common;

import java.time.LocalDateTime;

import lombok.Data;

/**
 * some args for signature
 * 
 * @author aoscoder (aoscoder@outlook.com)
 */
@Data
public class SignArg {

    Long headBlockNum;
    Long lastIrreversibleBlockNum;
    Long refBlockPrefix;
    LocalDateTime headBlockTime;
    String chainId;
    //
    int expiredSecond;

}
