package plus.aos.jaos.core.response.history.transaction;

import java.util.List;

import plus.aos.jaos.core.common.Action;
import lombok.Data;

/**
 * 
 * @author aoscoder (aoscoder@outlook.com)
 */
@Data
public class TransactionItem {

    private Long delaySec;
    private String expiration;
    private Long maxCpuUsageMs;
    private Long maxNetUsageWords;
    private Long refBlockNum;
    private Long refBlockPrefix;
    private List<Action> actions;
    private List<Action> contextFreeActions;
}
