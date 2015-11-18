package presentacion.control;

public class ControladorPrincipal {

	private static final String CREAR_CLIENTE = "../vista/crear-cliente.fxml";
	private static final String LISTAR_RESERVAS_SUCURSAL = "../vista/listar-reservas-sucursal.fxml";

	//TODO añadir constantes de tipo String para la vistas correspondientes a los casos de
	//uso Crear Reserva y Listar Reservas de una Sucursal
	private Stage primaryStage;

	@FXML
	void listarSucursales(ActionEvent event) throws LogicaExcepcion {
		initCasoDeUso(LISTAR_SUCURSALES, ControladorListarSucursales.class).show();
	}


}
