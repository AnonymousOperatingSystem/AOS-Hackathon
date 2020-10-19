package plus.aos.jaos.exception;

public class AosApiException extends RuntimeException {

    private ErrorCode aosErrorCode;

    public AosApiException(ErrorCode aosErrorCode) {
        this.aosErrorCode = aosErrorCode;
    }

    public AosApiException(String message, ErrorCode aosErrorCode) {
        super(message);
        this.aosErrorCode = aosErrorCode;
    }

    public AosApiException(Throwable cause, ErrorCode aosErrorCode) {
        super(cause);
        this.aosErrorCode = aosErrorCode;
    }

    public AosApiException(Throwable cause) {
        super(cause);
    }

    public AosApiException(String message, Throwable cause, ErrorCode aosErrorCode) {
        super(message, cause);
        this.aosErrorCode = aosErrorCode;
    }

    public ErrorCode getAosErrorCode() {
        return aosErrorCode;
    }

    public void setAosErrorCode(ErrorCode aosErrorCode) {
        this.aosErrorCode = aosErrorCode;
    }

}
